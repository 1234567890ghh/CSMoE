/*
imgui_ime_window.cpp
Copyright (C) 2020 Moemod Haoyuan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

extern "C" {
#include "common.h"
#include "client.h"
#include "gl_local.h"
#include "input_ime.h"
}

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_utils.h"
#include "imgui_imewindow.h"
#include <vector>
#include <string>
#include <algorithm>

static bool show_ime_candidate_list = true;

void ImGui_ImeWindow_Init()
{
	
}

// https://github.com/maildrop/DearImGui-with-IMM32/blob/master/IMM32IMGUI/imgui_imm32_onthespot.cpp
void ImGui_ImeWindow_OnGUI(void)
{
    ImGuiIO& io = ImGui::GetIO();

    static ImVec2 window_pos = ImVec2();
    static ImVec2 window_pos_pivot = ImVec2();

    /*
       #1 Candidate List Window ��λ�ä��v���뱣��
       Candidate List �򥯥�å������Ȥ��ˡ�������ɥ�λ�ä�Ӥ����ʤ���
       ����å���ˡ�TextInput��͎�������I��
       see #1
    */
    static ImGuiID candidate_window_root_id = 0;

    static ImGuiWindow* lastTextInputNavWindow = nullptr;
    static ImGuiID lastTextInputActiveId = 0;
    static ImGuiID lastTextInputFocusId = 0;

    // Candidate Window ��ե��`�������Ƥ���r�� Window ��λ�ä�������ʤ� 
    if (!(candidate_window_root_id &&
        ((ImGui::GetCurrentContext()->NavWindow ? ImGui::GetCurrentContext()->NavWindow->RootWindow->ID : 0u) == candidate_window_root_id))) {

        window_pos = ImVec2(ImGui::GetCurrentContext()->PlatformImePos.x + 1.0f,
            ImGui::GetCurrentContext()->PlatformImePos.y); // 
        window_pos_pivot = ImVec2(0.0f, 0.0f);

        const ImGuiContext* const currentContext = ImGui::GetCurrentContext();
        IM_ASSERT(currentContext || !"ImGui::GetCurrentContext() return nullptr.");
        if (currentContext) {
            // mouse press ���Ƥ��g�ϡ�ActiveID ���Ф�����Τǡ�
            if (!ImGui::IsMouseClicked(0)) {
                if ((currentContext->WantTextInputNextFrame != -1) ? (!!(currentContext->WantTextInputNextFrame)) : false) {
                    if ((!!currentContext->NavWindow) &&
                        (currentContext->NavWindow->RootWindow->ID != candidate_window_root_id) &&
                        (ImGui::GetActiveID() != lastTextInputActiveId)) {
                        lastTextInputNavWindow = ImGui::GetCurrentContext()->NavWindow;
                        lastTextInputActiveId = ImGui::GetActiveID();
                        lastTextInputFocusId = ImGui::GetFocusID();
                    }
                }
                else {
                    lastTextInputNavWindow = nullptr;
                    lastTextInputActiveId = 0;
                    lastTextInputFocusId = 0;
                }
            }
        }
    }

    int x, y;
    IME_GetInputScreenPos(&x, &y);
    ImVec2 target_screen_pos = ImVec2(x, y);

    std::vector<const char*> candidate_list;
    auto candidate_count = IME_GetCandidateListCount();
    for (size_t i = 0; i < candidate_count; ++i)
        candidate_list.push_back(IME_GetCandidate(i));

    auto selection = IME_GetCandidateListSelectedItem();
    auto candidate_window_num = IME_GetCandidateListPageSize();

    const char* completed = IME_GetCompositionString();

    /* Draw Candidate List */
    if (candidate_count || completed[0]) {

        std::vector<const char*> listbox_items = {};
        int candidate_selection = 0;

        /* �ک`���˷ָ�ޤ� */
        // TODO:candidate_window_num �΂���ע�� 0 ����ο����Ԥ����롣
        if (candidate_window_num)
        {
            int candidate_page = ((int)selection) / candidate_window_num;
            candidate_selection = ((int)selection) % candidate_window_num;

            auto begin_ite = std::begin(candidate_list);
            std::advance(begin_ite, candidate_page * candidate_window_num);
            auto end_ite = begin_ite;
            {
                auto the_end = std::end(candidate_list);
                for (int i = 0; end_ite != the_end && i < candidate_window_num; ++i) {
                    std::advance(end_ite, 1);
                }
            }

            std::copy(begin_ite, end_ite, std::back_inserter(listbox_items));
        }

        /* �⤷ candidate window ���������˳��뤳�Ȥ�����Τʤ�С��Ϥ˱�ʾ���� */
        const float candidate_window_width = 233;
        const float candidate_window_height =
            ((ImGui::GetStyle().FramePadding.y * 2) +
                ((ImGui::GetTextLineHeightWithSpacing()) * (std::min<int>(10, std::size(listbox_items)) + 3)));

        if (io.DisplaySize.y < (target_screen_pos.y + candidate_window_height)) {
            target_screen_pos.y -=
                ImGui::GetTextLineHeightWithSpacing() + candidate_window_height;
        }

        ImGui::SetNextWindowPos(target_screen_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowSize(ImVec2(0, candidate_window_height), ImGuiCond_Always);

        if (ImGui::Begin("##Overlay-IME-Candidate-List-Window",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing)) {
            if (ImGui::ListBoxHeader("##IMECandidateListWindow",
                static_cast<int>(std::size(listbox_items)),
                std::min<int>(10, std::size(listbox_items)))) {

                int i = 0; // for ������ǡ�i �򥤥󥯥���Ȥ��Ƥ���Τǡ�ע�� 
                for (const char*& listbox_item : listbox_items) {
                    ImGui::Text("%d.", i + IME_CandidateListStartsAtOne());
                    ImGui::SameLine();
                    if (ImGui::Selectable(listbox_item, (i == candidate_selection))) {

                        /* candidate list selection */

                        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                            !ImGui::IsAnyItemActive() &&
                            !ImGui::IsMouseClicked(0)) {
                            if (lastTextInputActiveId && lastTextInputFocusId) {
                                ImGui::SetActiveID(lastTextInputActiveId, lastTextInputNavWindow);
                                ImGui::SetFocusID(lastTextInputFocusId, lastTextInputNavWindow);
                            }
                        }

                        /*
                          ImmNotifyIME (hImc, NI_SELECTCANDIDATESTR, 0, candidate_page* candidate_window_num + i)); �򤷤����Τ�����
                          Vista �Խ� ImmNotifyIME �� NI_SELECTCANDIDATESTR �ϥ��ݩ`�Ȥ���ʤ���
                          @see IMM32���Q�����.doc from Microsoft

                          �����ǡ�DXUTguiIME.cpp (���Ĥ�ʹ���Ƥ��� DXUT �� gui IME �I�� �F�ڤϡ�deprecated �Q���ǡ�
                          https://github.com/microsoft/DXUT �Ǵ_�J������
                          ��ԓ�Υ��`�ɤϡ�https://github.com/microsoft/DXUT/blob/master/Optional/DXUTguiIME.cpp )
                          ��_�J�����Ȥ���

                          L.380���� �� Candidate List �򥯥�å������Ȥ��Υ��`�ɤ�����

                          �ɤ����Ƥ��뤫�Ȥ����� SendKey �ǡ�ʸӡ���`���륭�`���ͤ뤳�Ȥǡ�Candidate List������x�k���ФäƤ��롣
                          ���ʤ�Ȥ������ȣ�����

                          ���������� SendKey �����ä������`�ɤ����ɤ��롣
                        */
                        {
                            /*
                              ����ǡ��x�k���줿��Q���a��ĩβ�Έ��Ϥϴ_����
                              �����Ǥʤ����Ϥϡ��ΤΉ�Q�Ĺ����x�k���������ΤǤ��뤬��
                              keybd_event ��״�B���ͤäƤ���Τǡ�PostMessage �Ǥ��΄I����W�餻��
                              ���� request_candidate_list_str_commit �ϡ� WM_IME_COMPOSITION ������ǥ����å�����
                              WM_APP + 200 �� PostMessage ���ơ������ǌg�H�δ_���������Ф��롣
                            */

                            // TODO : ����
                            /*
                            if (candidate_selection == i) {
                                OutputDebugStringW(L"complete\n");
                                this->request_candidate_list_str_commit = 1;
                            }
                            else {
                                const BYTE nVirtualKey = (candidate_selection < i) ? VK_DOWN : VK_UP;
                                const size_t nNumToHit = abs(candidate_selection - i);
                                for (size_t hit = 0; hit < nNumToHit; ++hit) {
                                    keybd_event(nVirtualKey, 0, 0, 0);
                                    keybd_event(nVirtualKey, 0, KEYEVENTF_KEYUP, 0);
                                }
                                this->request_candidate_list_str_commit = (int)nNumToHit;
                            }
                            */
                        }

                    }

                    if (i == candidate_selection)
                        ImGui::SetItemDefaultFocus();
                    ++i;
                }
                ImGui::ListBoxFooter();
            }
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text(completed);
            ImGui::PopStyleColor();

            // #1 ���������륦����ɥ����ե��`������֤ä��Ȥ��ˤϡ�������ɥ���λ�ä������ƤϤ����ʤ���
            candidate_window_root_id = ImGui::GetCurrentWindowRead()->RootWindow->ID;
            ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
        }
        ImGui::End();
    }

    return;
}

/*
imgui_lcsm_warning.cpp
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

#include "imgui_lcsm_warning.h"
#include "imgui.h"

#include <algorithm>

static bool lcsm_enabled = true;

namespace ImGuiUtils {
	void CenterNextWindow(ImGuiCond cond = 0) {
		auto& io = ImGui::GetIO();
		const auto& ds = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(ds.x / 2, ds.y / 2), cond, ImVec2(0.5f, 0.5f));
	}
	ImVec2 GetScaledSize(ImVec2 in)
	{
		auto& io = ImGui::GetIO();
		float scale = std::max(1.0f, io.FontGlobalScale * 2);
		return { in.x * scale, in.y * scale };
	}
}

void ImGui_LCSM_OnGUI(void)
{
	if (!lcsm_enabled)
		return;

	ImGuiUtils::CenterNextWindow(ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImGuiUtils::GetScaledSize(ImVec2(640, 320)), ImGuiCond_Appearing);

	if (ImGui::Begin("CSMoE LCSM Warning", &lcsm_enabled, ImGuiWindowFlags_NoResize)) {
		ImGui::Text("Counter-Strike Mobile-oriented Edition (aka CSMoE or CSBTE-Mobile)");
		ImGui::Text("��Ȩ���У�BTE Team|CSMoE Team|�ٶ�csoldjb��");
		ImGui::Text("����Ϸ����GPLv3Э����ȫ��Դ��ѣ�����ʹ�ñ���Ϸ�����κ���ʽ��ӯ��");
		ImGui::Text("�������κ��ṩ�շѷ�����̼Ҿ��ǹٷ������ǲ��Էǹٷ��汾�ṩ�κμ���������");
		ImGui::Text("\t������κ����⣨������������������������������ban�š��ж�����ש����ը�ȣ�����Ը���");
		ImColor warning_color(1.0f, 0.2f, 0.2f, 1.0f);
		ImGui::TextColored(warning_color, "------------------�ر�����----------------");
		ImGui::TextColored(warning_color, "1.EC��Ӣ����ν��CS�Ƽ롱��CS����ʱ������Ϊ������Ϸ������ǿ��Ǵ�������Ϸ����Ϊ");
		ImGui::TextColored(warning_color, "2.Wilson�����������Σ���������ԭCS:BTE��Ϸ����Ȩ��������Ϸ����������Ȩ���Ա�ӯ��");
		ImGui::TextColored(warning_color, "�Ա����̣���������̳�|�Ա�����ح�͵��ݻ����ں�|BվID������������|����ID:liu1340567980");
		ImGui::TextColored(warning_color, "���ڴ˽�¶���Σ�һ����ȫ�Ĳ��С�ף������϶�ƴ���մ��룬��ֱ�Ӱ��������ĸ��¼�����Ϸ��");
		ImGui::TextColored(warning_color, "���������ԭCSBTE��Ȩ�����޷�άȨ��ϣ��������Ѳ����۾�����Ҫ���޳�С��ƭ�߽�Ǯ�����Σ�");
		ImGui::TextColored(warning_color, "3.CSMoE��ʹ�õ��زľ�Ϊ���������������������ѧϰ�����������ͣ��������غ�24Сʱ��ɾ����");
		ImGui::TextColored(warning_color, "------------------------------------------");
		/*
		ImGui::Text("�����·����룺����ͬ�⡹�������Ѿ���ȷ����������");
		ImGui::Text("�����밴�¡��˳�����ť�˳���Ϸ");

		static char buf[256];
		static bool enabled = 0;
		ImGui::InputTextWithHint("    ", "��ͬ��", buf, sizeof(buf), 0, [](ImGuiInputTextCallbackData *data){
			return 0;
		});

		if(ImGui::Selectable("������Ϸ", false, !!strcmp(buf, "��ͬ��") ? ImGuiSelectableFlags_Disabled : 0, ImGuiUtils::GetScaledSize({0, 36})))
		 */
		if (ImGui::Button("������Ϸ", ImGuiUtils::GetScaledSize({ 160, 36 }))) {
			lcsm_enabled = false;
		}
		ImGui::SameLine(ImGui::GetWindowSize().x / 2);
		if (ImGui::Button("�˳���Ϸ", ImGuiUtils::GetScaledSize({ 160, 36 }))) {
			exit(0);
		}
		ImGui::End();
	}
}
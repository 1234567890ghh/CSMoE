/*
imgui_cl_buy.cpp
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

#include "hud.h"
#include "cl_util.h"
#include "imgui.h"
#include "imgui_cl_buy.h"

namespace cl {

struct CImGuiBuyMenu::impl_t {
	bool draw = false;
};

CImGuiBuyMenu::CImGuiBuyMenu() : pimpl(std::make_unique<impl_t>()) {

}

CImGuiBuyMenu::~CImGuiBuyMenu() = default;

namespace ImGuiUtils {
	void CenterNextWindow(ImGuiCond cond = 0) {
		auto &io = ImGui::GetIO();
		const auto &ds = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(ds.x / 2, ds.y / 2), cond, ImVec2(0.5f, 0.5f));
	}
	ImVec2 GetScaledSize(ImVec2 in)
	{
		auto &io = ImGui::GetIO();
		float scale = std::max(1.0f, io.FontGlobalScale * 2);
		return {in.x * scale, in.y * scale};
	}
}

namespace ImGuiWpn{
	auto WeaponSection(const char *name)
	{
		return ImGui::Selectable(name, false, 0, ImGuiUtils::GetScaledSize({0, 36}));
	}

	auto WeaponSection(const char *name, const char *cmd)
	{
		auto btn = WeaponSection(name);
		if(btn)
		{
			ClientCmd(cmd);
		}
		return btn;
	}
}

void CImGuiBuyMenu::OnGUI() {
	if(!pimpl->draw)
		return;

	ImGuiUtils::CenterNextWindow();
	ImGui::SetNextWindowSize(ImGuiUtils::GetScaledSize(ImVec2(640, 480)));

	//ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, {0.0f, 0.5f});
	const bool CT = g_iTeamNumber != TEAM_TERRORIST;


	ImGui::Begin("��������");
	ImGui::Text("��������ʹ��");
	static int CurItem = 0;
	ImGui::SetNextItemOpen(CurItem == 1);
	if (ImGui::CollapsingHeader("��ǹ"))
	{
		ImGuiWpn::WeaponSection("1. USP45", "usp");
		ImGuiWpn::WeaponSection("2. Glock", "glock");
		ImGuiWpn::WeaponSection("3. P228", "p228");
		ImGuiWpn::WeaponSection("4. ɳĮ֮ӥ", "deagle");
		if(CT)
			ImGuiWpn::WeaponSection("5. FiveSeven", "fiveseven");
		else
			ImGuiWpn::WeaponSection("5. ˫��Elites", "elites");
		CurItem = 1;
	}
	ImGui::SetNextItemOpen(CurItem == 2);
	if (ImGui::CollapsingHeader("ɢ��ǹ"))
	{
		ImGuiWpn::WeaponSection("1. M3", "m3");
		ImGuiWpn::WeaponSection("2. XM1014", "xm1014");
		CurItem = 2;
	}
	ImGui::SetNextItemOpen(CurItem == 3);
	if (ImGui::CollapsingHeader("���ǹ"))
	{
		if(CT)
			ImGuiWpn::WeaponSection("1. TMP", "tmp");
		else
			ImGuiWpn::WeaponSection("1. MAC10", "mac10");
		ImGuiWpn::WeaponSection("2. MP5", "mp5");
		ImGuiWpn::WeaponSection("3. UMP45", "ump45");
		ImGuiWpn::WeaponSection("4. P90", "p90");
		CurItem = 3;
	}
	ImGui::SetNextItemOpen(CurItem == 4);
	if (ImGui::CollapsingHeader("�Զ���ǹ"))
	{
		if(CT)
		{
			ImGuiWpn::WeaponSection("1. FAMAS", "famas");
			ImGuiWpn::WeaponSection("2. SCOUT", "scout");
			ImGuiWpn::WeaponSection("3. M4A1", "m4a1");
			ImGuiWpn::WeaponSection("4. AUG", "aug");
			ImGuiWpn::WeaponSection("5. SG550", "sg550");
			ImGuiWpn::WeaponSection("6. AWP", "awp");
		}
		else
		{
			ImGuiWpn::WeaponSection("1. Galil", "galil");
			ImGuiWpn::WeaponSection("2. AK47", "ak47");
			ImGuiWpn::WeaponSection("3. SCOUT", "scout");
			ImGuiWpn::WeaponSection("4. SG552", "sg552");
			ImGuiWpn::WeaponSection("5. AWP", "awp");
			ImGuiWpn::WeaponSection("6. G3SG1", "g3sg1");

		}
		CurItem = 4;
	}
	ImGui::SetNextItemOpen(CurItem == 5);
	if (ImGui::CollapsingHeader("��ǹ"))
	{
		ImGuiWpn::WeaponSection("1. M249", "m249");
		CurItem = 5;
	}
	ImGuiWpn::WeaponSection("��������ҩ", "primammo");
	ImGuiWpn::WeaponSection("��������ҩ", "secammo");
	ImGui::SetNextItemOpen(CurItem == 8);
	if (ImGui::CollapsingHeader("װ��"))
	{
		ImGuiWpn::WeaponSection("1. ������", "vest");
		ImGuiWpn::WeaponSection("2. ������+����ͷ��", "vesthelm");
		ImGuiWpn::WeaponSection("3. ���ⵯ", "flash");
		ImGuiWpn::WeaponSection("4. �߱�����", "hegren");
		ImGuiWpn::WeaponSection("5. ����", "sgren");
		if(CT)
		{
			ImGuiWpn::WeaponSection("6. ����", "defuser");
			ImGuiWpn::WeaponSection("7. ҹ����", "nvgs");
			//ImGuiWpn::WeaponSection("8. ������", "shield");
		}
		else
		{
			ImGuiWpn::WeaponSection("6. ҹ����", "nvgs");
		}
		CurItem = 8;
	}
	ImGui::SetNextItemOpen(CurItem == 9);
	if (ImGui::CollapsingHeader("��������"))
	{
		ImGuiWpn::WeaponSection("1. �����̵�");
		CurItem = 9;
	}
	if (ImGuiWpn::WeaponSection("�ر�"))
		pimpl->draw = false;

	//ImGui::PopStyleVar(ImGuiStyleVar_SelectableTextAlign);

	ImGui::End();
}

}
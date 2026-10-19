// REBS_MWGSM_TraderExt - Mission Server
// 5_Mission Layer - Boots the static trader manager after the parent mod

modded class MissionServer
{
	private ref REBS_StaticTraderManager m_REBS_StaticManager;

	override void OnInit()
	{
		super.OnInit();

		// Parent MWGSM_RoamingTrader already initialises its own manager.
		// We only start the static extension.
		m_REBS_StaticManager = REBS_StaticTraderManager.GetInstance();

		// Small delay so the parent config + world are fully ready
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(REBS_InitStaticTraders, 2500, false);
	}

	void REBS_InitStaticTraders()
	{
		if (m_REBS_StaticManager)
			m_REBS_StaticManager.Initialize();
	}
}

#include "Achievements.h"


/*std::unique_ptr<CSteamAchievements> Achievements::m_pSteamAchievements = NULL;

Achievement_t Achievements::g_Achievements[4]{
		_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter")
};*/

AchievementProps Achievements::g_Achievements[2]{
	{"WinOneGame", false},
	{"WinAHundredGames", false}
};

Achievements::~Achievements()
{
    for (auto& pSubject : m_pVecObservedSubjects)
    {
        pSubject->RemoveObserver(this);
    }
}

void Achievements::Notify(ScoreUIComponent* pScoreUIComponent)
{
	if (!g_Achievements[0].achieved && pScoreUIComponent->GetScore() >= 500)
	{
		std::cout << g_Achievements[0].name << '\n';
		g_Achievements[0].achieved = true;
	}
}

void Achievements::AddSubjectPointer(dae::Subject<ScoreUIComponent>* pScoreUIComponent)
{
    m_pVecObservedSubjects.emplace_back(pScoreUIComponent);
}

/*void Achievements::SetSteamAchievements(CSteamAchievements* pSteamAchievements)
{
	m_pSteamAchievements.reset(pSteamAchievements);
}*/


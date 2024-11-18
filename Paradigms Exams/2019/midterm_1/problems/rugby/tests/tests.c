#include "../rugby.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gl/lunit/lunit.h"

typedef struct {
  int num_teams;
  int num_games;
  char** teams;
  int** games;
} TestData;

void TestDataInit(TestData* data, int num_teams, int num_games) {
  data->num_teams = num_teams;
  data->num_games = num_games;
  data->teams = malloc(num_teams * sizeof(char*));
  CHECK(data->teams != NULL);
  data->games = malloc(num_games * sizeof(int*));
  CHECK(data->games != NULL);
  for (int i = 0; i < num_games; ++i) {
    data->games[i] = malloc(4 * sizeof(int));
    CHECK(data->games[i] != NULL);
  }
}

void TestDataDispose(TestData* data) {
  for (int i = 0; i < data->num_teams; ++i) {
    free(data->teams[i]);
  }
  free(data->teams);
  for (int i = 0; i < data->num_games; ++i) {
    free(data->games[i]);
  }
  free(data->games);
}

void Game(int* game, int team_a, int team_b, int points_a, int points_b) {
  game[0] = team_a;
  game[1] = team_b;
  game[2] = points_a;
  game[3] = points_b;
}

char* AddGame(char* results, char* team_a, int points_a, int points_b,
	      char* team_b) {
  CHECK(0 < points_a && points_a < 256);
  CHECK(0 < points_b && points_b < 256);
  for (int i = 0; i < strlen(team_a); ++i) {
    CHECK(('a' <= team_a[i] && team_a[i] <= 'z') ||
	  ('A' <= team_a[i] && team_a[i] <= 'Z'));
  }
  for (int i = 0; i < strlen(team_b); ++i) {
    CHECK(('a' <= team_b[i] && team_b[i] <= 'z') ||
	  ('A' <= team_b[i] && team_b[i] <= 'Z'));
  }
  int len = strlen(results);
  if (len > 0) {
    results = realloc(results, len + 2);
    results[len] = ';';
    len++;
    results[len] = '\0';
  }
  int len_a = strlen(team_a);
  int len_b = strlen(team_b);
  results = realloc(results, len + len_a + len_b + 5);
  sprintf(results + len, "%s %c%c %s", team_a, (char)points_a,
	  (char)points_b, team_b);
  return results;
}

char* Calculate(int num_teams, int num_games, char* teams[], int** games,
		int** p, int** g, int** w) {
  char* results = malloc(1);
  results[0] = '\0';
  int num_bytes = num_teams * sizeof(int);
  if (p != NULL) {
    *p = malloc(num_bytes);
    *g = malloc(num_bytes);
    *w = malloc(num_bytes);
    CHECK(p != NULL && g != NULL && w != NULL);
    memset(*p, 0, num_bytes);
    memset(*g, 0, num_bytes);
    memset(*w, 0, num_bytes);
  }
  for (int i = 0; i < num_games; ++i) {
    int* game = games[i];
    results = AddGame(results, teams[game[0]], game[2], game[3],
		      teams[game[1]]);
    if (p == NULL) {
      continue;
    }
    (*p)[game[0]] += game[2];
    (*p)[game[1]] += game[3];
    (*g)[game[0]]++;
    (*g)[game[1]]++;
    if (game[2] > game[3]) {
      (*w)[game[0]]++;
    } else if (game[2] < game[3]) {
      (*w)[game[1]]++;
    }
  }
  return results;
}

void TestTotalGamesPlayed(Test* test, TestData data) {
  int *p, *g, *w;
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, &p, &g, &w);
  for (int i = 0; i < data.num_teams; ++i) {
    LOG_INFO("Checking TotalGamesPlayed for %s", data.teams[i]);
    ASSERT_INT_EQ(TotalGamesPlayed(results, data.teams[i]), g[i]);
  }
  free(results);
  free(p);
  free(g);
  free(w);
  TestDataDispose(&data);
}

void TestTotalPointsScored(Test* test, TestData data) {
  int *p, *g, *w;
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, &p, &g, &w);
  for (int i = 0; i < data.num_teams; ++i) {
    LOG_INFO("Checking TotalPointsScored for %s", data.teams[i]);
    ASSERT_INT_EQ(TotalPointsScored(results, data.teams[i]), p[i]);
  }
  free(results);
  free(p);
  free(g);
  free(w);
  TestDataDispose(&data);
}

void TestNumWins(Test* test, TestData data) {
  int *p, *g, *w;
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, &p, &g, &w);
  for (int i = 0; i < data.num_teams; ++i) {
    LOG_INFO("Checking NumWins for %s", data.teams[i]);
    ASSERT_INT_EQ(NumWins(results, data.teams[i]), w[i]);
  }
  free(results);
  free(p);
  free(g);
  free(w);
  TestDataDispose(&data);
}

// Two teams

TestData TwoTeamsData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("xyz");
  Game(data.games[0], 0, 1, 1, 2);
  return data;
}

TEST(TwoTeams_TGP) {
  TestTotalGamesPlayed(test, TwoTeamsData());
}

TEST(TwoTeams_TPS) {
  TestTotalPointsScored(test, TwoTeamsData());
}

TEST(TwoTeams_NW) {
  TestNumWins(test, TwoTeamsData());
}

// Three teams

TestData ThreeTeamsData() {
  TestData data;
  TestDataInit(&data, 3, 2);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("xyz");
  data.teams[2] = strdup("jkl");
  Game(data.games[0], 0, 1, 5, 2);
  Game(data.games[1], 1, 2, 1, 64);
  return data;
}

TEST(ThreeTeams_TGP) {
  TestTotalGamesPlayed(test, ThreeTeamsData());  
}

TEST(ThreeTeams_TPS) {
  TestTotalPointsScored(test, ThreeTeamsData());  
}

TEST(ThreeTeams_NW) {
  TestNumWins(test, ThreeTeamsData());  
}

// Long names

TestData LongNamesData() {
  TestData data;
  TestDataInit(&data, 3, 6);
  data.teams[0] = strdup("abcakjsdhqweasdfsdbxcvbsdfg");
  data.teams[1] = strdup("wergsbfsdhfdgbdfgxyzasdfguwyegbsmnd");
  data.teams[2] = strdup("jklqlehadfmasdfwesdfgfgdncvgbsresdf");
  Game(data.games[0], 0, 1, 1, 2);
  Game(data.games[1], 1, 2, 2, 3);
  Game(data.games[2], 1, 0, 5, 6);
  Game(data.games[3], 2, 1, 3, 4);
  Game(data.games[4], 2, 0, 1, 1);
  Game(data.games[5], 1, 0, 6, 7);
  return data;
}

TEST(LongNames_TGP) {
  TestTotalGamesPlayed(test, LongNamesData());
}

TEST(LongNames_TPS) {
  TestTotalPointsScored(test, LongNamesData());  
}

TEST(LongNames_NW) {
  TestNumWins(test, LongNamesData());  
}

// Single letter names

TestData SingleLettersData() {
  TestData data;
  TestDataInit(&data, 7, 7 * 6);
  data.teams[0] = strdup("a");
  data.teams[1] = strdup("b");
  data.teams[2] = strdup("c");
  data.teams[3] = strdup("d");
  data.teams[4] = strdup("e");
  data.teams[5] = strdup("f");
  data.teams[6] = strdup("g");
  int cnt = 0;
  for (int i = 0; i < 7; ++i) {
    for (int j = 0; j < 7; ++j) {
      if (i == j) {
	continue;
      }
      Game(data.games[cnt], i, j,
	   (i * 7 + j * 13) % 127 + 1,
	   (i * 13 + j * 5) % 127 + 1);
      cnt++;
    }
  }
  return data;
}

TEST(SingleLetterNames_TGP) {
  TestTotalGamesPlayed(test, SingleLettersData());  
}

TEST(SingleLetterNames_TPS) {
  TestTotalPointsScored(test, SingleLettersData());  
}

TEST(SingleLetterNames_NW) {
  TestNumWins(test, SingleLettersData());  
}

// All draws

TestData AllDrawsData() {
  int num_teams = 20;
  int num_games = num_teams * (num_teams - 1);
  TestData data;
  TestDataInit(&data, num_teams, num_games);
  for (int i = 0; i < num_teams; i++) {
    data.teams[i] = malloc(10);
    assert(data.teams[i] != NULL);
    sprintf(data.teams[i], "%c%c%c", 65 + i, 66 + i, 67 + i);
  }
  int cnt = 0;
  for (int i = 0; i < num_teams; ++i) {
    for (int j = 0; j < num_teams; ++j) {
      if (i == j) {
	continue;
      }
      data.games[cnt][0] = i;
      data.games[cnt][1] = j;
      data.games[cnt][2] = 10;
      data.games[cnt][3] = 10;
      ++cnt;
    }
  }
  return data;
}
  
TEST(AllDraws_TGP) {
  TestTotalGamesPlayed(test, AllDrawsData());
}

TEST(AllDraws_TPS) {
  TestTotalPointsScored(test, AllDrawsData());
}

TEST(AllDraws_NW) {
  TestNumWins(test, AllDrawsData());
}

// Prefix two teams

TestData PrefixTwoTeamsData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("a");
  data.teams[1] = strdup("ab");
  Game(data.games[0], 0, 1, 5, 7);
  return data;
}

TEST(PrefixTwoTeams_TGP) {
  TestTotalGamesPlayed(test, PrefixTwoTeamsData());
}

TEST(PrefixTwoTeams_TPS) {
  TestTotalPointsScored(test, PrefixTwoTeamsData());
}

TEST(PrefixTwoTeams_NW) {
  TestNumWins(test, PrefixTwoTeamsData());
}

// Unknown team

TestData UnknownTeamData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("a");
  data.teams[1] = strdup("b");
  Game(data.games[0], 0, 1, 1, 1);
  return data;
}

TEST(UnknownTeam_TGP) {
  TestData data = UnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "c"), 0);
  free(results);
  TestDataDispose(&data);
}

TEST(UnknownTeam_TPS) {
  TestData data = UnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(TotalPointsScored(results, "c"), 0);
  free(results);
  TestDataDispose(&data);
}

TEST(UnknownTeam_NW) {
  TestData data = UnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(NumWins(results, "c"), 0);
  free(results);
  TestDataDispose(&data);
}

// Prefix unknown team

TestData PrefixUnknownTeamData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("qwe");
  Game(data.games[0], 0, 1, 1, 1);
  return data;
}

TEST(PrefixUnknownTeam_TGP) {
  TestData data = PrefixUnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "a"), 0);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "ab"), 0);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "abcd"), 0);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "q"), 0);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "qw"), 0);
  ASSERT_INT_EQ(TotalGamesPlayed(results, "qwet"), 0);  
  free(results);
  TestDataDispose(&data);
}

TEST(PrefixUnknownTeam_TPS) {
  TestData data = PrefixUnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(TotalPointsScored(results, "a"), 0);
  ASSERT_INT_EQ(TotalPointsScored(results, "ab"), 0);
  ASSERT_INT_EQ(TotalPointsScored(results, "abcd"), 0);
  ASSERT_INT_EQ(TotalPointsScored(results, "q"), 0);
  ASSERT_INT_EQ(TotalPointsScored(results, "qw"), 0);
  ASSERT_INT_EQ(TotalPointsScored(results, "qwed"), 0);  
  free(results);
  TestDataDispose(&data);
}

TEST(PrefixUnknownTeam_NW) {
  TestData data = PrefixUnknownTeamData();
  char* results = Calculate(data.num_teams, data.num_games, data.teams,
			    data.games, NULL, NULL, NULL);
  ASSERT_INT_EQ(NumWins(results, "a"), 0);
  ASSERT_INT_EQ(NumWins(results, "ab"), 0);
  ASSERT_INT_EQ(NumWins(results, "abcd"), 0);
  ASSERT_INT_EQ(NumWins(results, "q"), 0);
  ASSERT_INT_EQ(NumWins(results, "qw"), 0);
  ASSERT_INT_EQ(NumWins(results, "qwed"), 0);  
  free(results);
  TestDataDispose(&data);
}

// Sign bit both

TestData SignBitBothData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("xyz");
  Game(data.games[0], 0, 1, 128, 129);
  return data;
}

TEST(SignBitBoth_TGP) {
  TestTotalGamesPlayed(test, SignBitBothData());
}

TEST(SignBitBoth_TPS) {
  TestTotalPointsScored(test, SignBitBothData());
}

TEST(SignBitBoth_NW) {
  TestNumWins(test, SignBitBothData());
}

// Sign bit left

TestData SignBitLeftData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("xyz");
  Game(data.games[0], 0, 1, 128, 1);
  return data;
}

TEST(SignBitLeft_TGP) {
  TestTotalGamesPlayed(test, SignBitLeftData());
}

TEST(SignBitLeft_TPS) {
  TestTotalPointsScored(test, SignBitLeftData());
}

TEST(SignBitLeft_NW) {
  TestNumWins(test, SignBitLeftData());
}

// Sign bit right

TestData SignBitRightData() {
  TestData data;
  TestDataInit(&data, 2, 1);
  data.teams[0] = strdup("abc");
  data.teams[1] = strdup("xyz");
  Game(data.games[0], 0, 1, 1, 255);
  return data;
}

TEST(SignBitRight_TGP) {
  TestTotalGamesPlayed(test, SignBitRightData());
}

TEST(SignBitRight_TPS) {
  TestTotalPointsScored(test, SignBitRightData());
}

TEST(SignBitRight_NW) {
  TestNumWins(test, SignBitRightData());
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);
  
  TEST_SUITE_WITH_WEIGHT(basic, 0.8);
  TEST_SUITE_WITH_WEIGHT(prefix, 0.05);
  TEST_SUITE_WITH_WEIGHT(sign_bit, 0.15);
    
  ADD_TEST(basic, TwoTeams_TGP);
  ADD_TEST(basic, TwoTeams_TPS);
  ADD_TEST(basic, TwoTeams_NW);
  
  ADD_TEST(basic, ThreeTeams_TGP);
  ADD_TEST(basic, ThreeTeams_TPS);
  ADD_TEST(basic, ThreeTeams_NW);
  
  ADD_TEST(basic, LongNames_TGP);
  ADD_TEST(basic, LongNames_TPS);
  ADD_TEST(basic, LongNames_NW)
    
  ADD_TEST(basic, SingleLetterNames_TGP);
  ADD_TEST(basic, SingleLetterNames_TPS);
  ADD_TEST(basic, SingleLetterNames_NW);
  
  ADD_TEST(basic, AllDraws_TGP);
  ADD_TEST(basic, AllDraws_TPS);  
  ADD_TEST(basic, AllDraws_NW);
  
  ADD_TEST(basic, UnknownTeam_TGP);
  ADD_TEST(basic, UnknownTeam_TPS);
  ADD_TEST(basic, UnknownTeam_NW);

  ADD_TEST(prefix, PrefixTwoTeams_TGP);
  ADD_TEST(prefix, PrefixTwoTeams_TPS);
  ADD_TEST(prefix, PrefixTwoTeams_NW);

  ADD_TEST(prefix, PrefixUnknownTeam_TGP);
  ADD_TEST(prefix, PrefixUnknownTeam_TPS);
  ADD_TEST(prefix, PrefixUnknownTeam_NW);

  ADD_TEST(sign_bit, SignBitBoth_TGP);
  ADD_TEST(sign_bit, SignBitBoth_TPS);
  ADD_TEST(sign_bit, SignBitBoth_NW);

  ADD_TEST(sign_bit, SignBitLeft_TGP);
  ADD_TEST(sign_bit, SignBitLeft_TPS);
  ADD_TEST(sign_bit, SignBitLeft_NW);  

  ADD_TEST(sign_bit, SignBitRight_TGP);
  ADD_TEST(sign_bit, SignBitRight_TPS);
  ADD_TEST(sign_bit, SignBitRight_NW);  
  
  TestSuite* suites[] = {&basic, &prefix, &sign_bit};
  ProcessTestSuites(3, suites, &opts);
  TestSuiteDispose(&basic);
  TestSuiteDispose(&prefix);
  TestSuiteDispose(&sign_bit);
  return 0;
}

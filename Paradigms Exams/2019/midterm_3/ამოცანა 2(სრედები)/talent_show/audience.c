#include "audience.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void AudienceJurorEvaluatedParticipant(Audience* audience, int juror_id,
				       int participant_id, bool yes) {
  sleep(rand() % 2);
  audience->yes[juror_id][participant_id] = yes;
  printf("Juror %d Participant %d: %s\n", juror_id, participant_id, yes ? "YES" : "NO");
}

void AudienceJuryAnnounceParticipantScore(Audience* audience,
					  int participant_id, int num_yes) {
  sleep(rand() % 2);
  audience->score[participant_id] = num_yes;
  printf("Participant %d got %d yes\n", participant_id, num_yes);
}

void AudienceJurorGreet(Audience* audience, int juror_id) {
  sleep(rand() % 2);  
  printf("Juror %d greetings\n", juror_id);
}

void AudienceJuryGreet(Audience* audience) {
  sleep(rand() % 2);
  printf("Jury greetings\n");
}

void AudienceJurorDone(Audience* audience, int juror_id) {
  sleep(rand() % 2);
  printf("Juror done: %d\n", juror_id);
}

void AudienceJuryDone(Audience* audience) {
  sleep(rand() % 2);
  printf("Jury done\n");
}

void AudienceJuryAnnounceNewParticipant(Audience* audience,
					int participant_id) {
  sleep(rand() % 2);
  printf("New participant: %d\n", participant_id);
}

void AudienceInit(Audience* audience) {
  for (int j = 0; j < 105; ++j) {
    audience->score[j] = 0;
    for (int i = 0; i < 15; ++i) {
      audience->yes[i][j] = false;
    }
  }
}

bool AudienceCheckScores(Audience* audience, int num_jurors, int num_participants) {
  for (int i = 0; i < num_participants; ++i) {
    int score = 0;
    for (int j = 0; j < num_jurors; ++j) {
      score += audience->yes[j][i];
    }
    if (score != audience->score[i]) {
      printf("Incosistent score was reported on participant: %d\n", i);
      printf("Counted %d but was reported %d\n", score, audience->score[i]);
      printf("Individual jurors reported:\n");
      for (int j = 0; j < num_jurors; ++j) {
	printf("%d ", audience->yes[i][j]);
      }
      printf("\n");
      return false;
    }
  }
  return true;
}

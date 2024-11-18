#include "sample_talent_show.h"

#include <stdlib.h>

void* JurorStart(void* args) {
  JurorArgs* juror = args;
  Jury* jury = juror->jury;
  // Fill in.
  return NULL;
}

void* JuryStart(void* args) {
  Jury* jury = args;
  // Fill in.
  return NULL;
}

void JurorInit(JurorArgs* juror, int id, Jury* jury) {
  juror->id = id;
  juror->jury = jury;
  // Initialize mutexes and semaphores if needed.
}

void JurorDestroy(JurorArgs* juror) {
  // Destroy mutexes and semaphores if needed.
}

void JuryInit(Jury* jury, Audience* audience, Evaluator evaluator,
	      int num_jurors) {
  jury->audience = audience;
  jury->evaluator = evaluator;
  jury->num_jurors = num_jurors;
  jury->num_participants = 0;
  jury->take_no_new_participants = false;
  // Initialize mutexes and semaphores if needed.
}

void JuryDestroy(Jury* jury) {
  for (int i = 0; i < jury->num_jurors; ++i) {
    JurorDestroy(&jury->jurors[i]);
  }
  // Destroy mutexes and semaphores if needed.
}

void JuryStartAcceptingParticipants(Jury* jury) {
  pthread_t tid;
  for (int i = 0; i < jury->num_jurors; ++i) {
    JurorInit(&jury->jurors[i], i, jury);
    pthread_create(&tid, NULL, JurorStart, &jury->jurors[i]);
    pthread_detach(tid);
  }
  pthread_create(&tid, NULL, JuryStart, jury);
  pthread_detach(tid);
}

bool JuryTakeNewParticipant(Jury* jury) {
  // Fill in.
  return false;
}

void JuryStopAcceptingParticipants(Jury* jury) {
  // Fill in.
}

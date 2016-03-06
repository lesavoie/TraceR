//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
//
// Written by:
//     Nikhil Jain <nikhil.jain@acm.org>
//     Bilge Acun <acun2@illinois.edu>
//     Abhinav Bhatele <bhatele@llnl.gov>
//
// LLNL-CODE-681378. All rights reserved.
//
// This file is part of TraceR. For details, see:
// https://github.com/LLNL/tracer
// Please also read the LICENSE file for our notice and the LGPL.
//////////////////////////////////////////////////////////////////////////////

#include "assert.h"
#include "Task.h"
#include "PE.h"

Task::Task() {
  backwDepSize = 0;
  forwDepSize = 0;
  backwardDep = 0;
  forwardDep = 0;
  myEntries = 0;
  done = false;
  execTime = -1;
  endEvent = 0;
  msgEntCount = 0;
  bgPrintCount = 0;
}

void Task::printEvt(tw_lp * lp, double startTime, int PEno, int jobNo)
{
  for(int i = 0; i < bgPrintCount; i++) {
    myBgPrints[i].print(lp, startTime, PEno, jobNo);
  }
}

Task::~Task()
{
  delete[] forwardDep;
  delete[] backwardDep;
  delete[] myBgPrints;
  delete[] myEntries;
}

void Task::copyFromTask(Task* t)
{
  execTime= t->execTime;
  done = t->done;
  myMsgId.pe = t->myMsgId.pe;
  myMsgId.id = t->myMsgId.id;
  charmEP = t->charmEP;
  msgEntCount=t->msgEntCount;
  myEntries=new MsgEntry[msgEntCount];

  for(int i=0; i< msgEntCount; i++)
  {
    myEntries[i].msgId.id = t->myEntries[i].msgId.id;
    myEntries[i].msgId.pe = t->myEntries[i].msgId.pe;
    myEntries[i].msgId.size = t->myEntries[i].msgId.size;
    myEntries[i].node = t->myEntries[i].node;
    myEntries[i].thread = t->myEntries[i].thread;
    // sendTime is absolute
    myEntries[i].sendOffset = t->myEntries[i].sendOffset;

  }

  backwDepSize = t->backwDepSize;
  backwardDep=new int[backwDepSize];
  for(int i=0; i< backwDepSize; i++)
  {
    backwardDep[i] = t->backwardDep[i];
  }

  forwDepSize = t->forwDepSize;
  forwardDep=new int[forwDepSize];
  for(int i=0; i<forwDepSize; i++){
    forwardDep[i] = t->forwardDep[i];
  }

  bgPrintCount = t->bgPrintCount;
  myBgPrints = new BgPrint[bgPrintCount];
  for(int i=0; i<bgPrintCount; i++) {
    myBgPrints[i].copy(&t->myBgPrints[i]);
  }
}

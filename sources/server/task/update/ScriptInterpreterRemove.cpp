#include "stdafx.h"
#include "ScriptInterpreterRemove.h"
#include <shared/Log.h>

#include "update/worker/ScriptInterpreter.h"

namespace task { namespace update {

   std::string CScriptInterpreterRemove::m_taskName = "scriptInterpreter.remove";

   CScriptInterpreterRemove::CScriptInterpreterRemove(const std::string & scriptInterpreterName)
      :m_scriptInterpreterName(scriptInterpreterName)
   {
   }

   CScriptInterpreterRemove::~CScriptInterpreterRemove()
   {
   }

   const std::string & CScriptInterpreterRemove::getName()
   {
      return m_taskName;
   }

   bool CScriptInterpreterRemove::doWork(TaskProgressFunc pFunctor)
   {
      YADOMS_LOG(information) << "Start removing scriptInterpreter...";

      //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
      ::update::worker::CScriptInterpreter::WorkerProgressFunc adapter = boost::bind(pFunctor, _1, _2, _3);

      ::update::worker::CScriptInterpreter worker(adapter);
      worker.remove(m_scriptInterpreterName);

      YADOMS_LOG(information) << "End of removing scriptInterpreter";
      return true;
   }

} //namespace update
} //namespace task

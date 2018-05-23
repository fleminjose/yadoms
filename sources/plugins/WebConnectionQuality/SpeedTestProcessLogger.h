#pragma once
#include <shared/process/IExternalProcessLogger.h>

//TODO mettre dans shared ?

class CSpeedTestProcessLogger : public shared::process::IExternalProcessLogger
{
public:
   explicit CSpeedTestProcessLogger(const std::string& logPrefix);
   virtual ~CSpeedTestProcessLogger();

   // IExternalProcessLogger Implementation
   void init() override;
   void information(const std::string& line) override;
   void error(const std::string& line) override;
   void purgeLogFile() override;
   // [END] IExternalProcessLogger Implementation

private:
   const std::string m_logPrefix;
};

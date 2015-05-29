#include "stdafx.h"
#include "Plugin.h"
#include <shared/Log.h>

#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "update/info/UpdateSite.h"

#include "WorkerTools.h"
#include <Poco/File.h>

#include "pluginSystem/Manager.h"

namespace update {
   namespace worker {

      CPlugin::CPlugin(WorkerProgressFunc progressCallback)
         :m_progressCallback(progressCallback)
      {

      }


      CPlugin::~CPlugin()
      {

      }


      void CPlugin::install(const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Installing new plugin from " + downloadUrl);

         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package");
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success");

            /////////////////////////////////////////////
            //2. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString());
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success");

               m_progressCallback(true, 90.0f, "Refresh plugin list");
               boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
               if (pluginManager)
                  pluginManager->updatePluginList();
               m_progressCallback(true, 100.0f, "Plugin installed with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what());
         }
  
      }

      void CPlugin::update(const std::string & pluginName, const std::string & downloadUrl)
      {
         m_progressCallback(true, 0.0f, "Updating plugin " + pluginName + " from " + downloadUrl);
         /////////////////////////////////////////////
         //1. download package
         /////////////////////////////////////////////
         try
         {

            m_progressCallback(true, 0.0f, "Downloading package");
            Poco::Path downloadedPackage = CWorkerTools::downloadPackage(downloadUrl);
            m_progressCallback(true, 50.0f, "Package downloaded with success");

            /////////////////////////////////////////////
            //2. stop any instance
            /////////////////////////////////////////////
            boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
            if (pluginManager)
               pluginManager->stopAllInstancesOfPlugin(pluginName);

            /////////////////////////////////////////////
            //3. deploy package
            /////////////////////////////////////////////
            try
            {
               m_progressCallback(true, 50.0f, "Deploy package " + downloadedPackage.toString());
               Poco::Path pluginPath = CWorkerTools::deployPluginPackage(downloadedPackage);
               m_progressCallback(true, 90.0f, "Plugin deployed with success");

               m_progressCallback(true, 90.0f, "Start instances");
               if (pluginManager)
                  pluginManager->startAllInstancesOfPlugin(pluginName);
               m_progressCallback(true, 100.0f, "Plugin updated with success");
            }
            catch (std::exception & ex)
            {
               //fail to extract package file
               m_progressCallback(false, 100.0f, std::string("Fail to deploy package : ") + ex.what());
            }


            //delete downloaded zip file
            Poco::File toDelete(downloadedPackage.toString());
            if (toDelete.exists())
               toDelete.remove();

         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to download package : ") + ex.what());
         }
      }

      void CPlugin::remove(const std::string & pluginName)
      {
         m_progressCallback(true, 0.0f, "Removing plugin " + pluginName);

         try
            {
            /////////////////////////////////////////////
            //1. stop any instance
            /////////////////////////////////////////////
            boost::shared_ptr<pluginSystem::CManager> pluginManager = shared::CServiceLocator::instance().get<pluginSystem::CManager>();
            if (pluginManager)
               pluginManager->stopAllInstancesOfPlugin(pluginName);

            /////////////////////////////////////////////
            //2. remove plugin folder
            /////////////////////////////////////////////
            boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
            Poco::Path pluginPath(startupOptions->getPluginsPath());
            pluginPath.append(pluginName);

            Poco::File toDelete(pluginPath);
            if (toDelete.exists())
               toDelete.remove(true);

            /////////////////////////////////////////////
            //3. update plugin manager
            /////////////////////////////////////////////
            if (pluginManager)
               pluginManager->updatePluginList();
         }
         catch (std::exception & ex)
         {
            //fail to download package
            m_progressCallback(false, 100.0f, std::string("Fail to delete plugin : ") + pluginName + " : " + ex.what());
         }
      }









   } // namespace worker
} // namespace update

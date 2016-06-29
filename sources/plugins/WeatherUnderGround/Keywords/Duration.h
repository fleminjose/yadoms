#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Duration class
//--------------------------------------------------------------
class CDuration : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                      Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] keyWordName          The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CDuration(std::string pluginName,
             std::string keyWordName);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CDuration();

   //--------------------------------------------------------------
   /// \brief	                      Set the value from the container
   /// \param[in] valueContainer       The container where the value is stored
   /// \param[in] filter               The name of the information into the container
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   void setValue(const shared::CDataContainer& valueContainer,
                 const std::string& filter) const;

   // IKeyword implementation
   void initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                   shared::CDataContainer details) const override;
   boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const override;
   // [END] IKeyword implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	The duration (boost::time_duration)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CDuration> m_duration;
};


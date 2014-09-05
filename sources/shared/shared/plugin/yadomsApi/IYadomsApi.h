#pragma once
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/information/IInformation.h>
#include <shared/StringExtension.h>
#include <shared/DataContainer.h>
#include "IDeviceCommand.h"
#include "IManuallyDeviceCreationData.h"
#include "IManuallyDeviceCreationTestData.h"
#include <shared/plugin/yadomsApi/StandardCapacity.h>
#include <shared/plugin/yadomsApi/KeywordAccessMode.h>
#include <shared/plugin/yadomsApi/KeywordType.h>

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief The API used by the plugins to interact with Yadoms
   //-----------------------------------------------------
   class IYadomsApi
   {
   public:
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~IYadomsApi() {}

      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Events
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      //-----------------------------------------------------
      ///\brief Events Id from Yadoms received by the plugin
      //-----------------------------------------------------
      enum
      {
         kEventUpdateConfiguration = shared::event::kUserFirstId, // Yadoms notify the plugin that its configuration was changed
         kEventDeviceCommand,                                     // Yadoms send a command to a device managed by this plugin
         kEventManuallyDeviceCreationTest,                        // Yadoms ask the plugin to test a device with provided parameters, before to create it
         kEventManuallyDeviceCreation,                            // Yadoms ask the plugin to create a device
         kPluginFirstEventId                                      // The next usable event ID for the plugin code
      };

      //-----------------------------------------------------
      ///\brief Get the event handler associated to the plugin. The event handler is used
      ///       to received, wait for, or post events from/to Yadoms
      ///\return The plugin event handler
      //-----------------------------------------------------   
      virtual shared::event::CEventHandler & getEventHandler() = 0;

      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Devices
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      //-----------------------------------------------------
      ///\brief Check if a device already exists for the server
      ///\param    [in]    device            The device name
      ///\return true if the device exists, false if not
      //-----------------------------------------------------
      virtual bool deviceExists(const std::string& device) const = 0;

      //-----------------------------------------------------
      ///\brief Get the details of a device
      ///\param    [in]    device            The device name
      ///\return the device details
      ///\throw shared::exception::CEmptyResult if device dosen't exist
      //-----------------------------------------------------
      virtual const shared::CDataContainer getDeviceDetails(const std::string& device) const = 0;

      //-----------------------------------------------------
      ///\brief Declare a device
      ///\param    [in]    device             The device name
      ///\param    [in]    model              The device model or description (ex : "Oregon Scientific CN185")
      ///\param    [in]    details            A free string used by plugin
      ///\return true if the device has been successfully created, false if already exist
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
	  virtual bool declareDevice(const std::string& device, const std::string& model, const std::string & details = shared::CStringExtension::EmptyString) = 0;
      
      
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Keywords
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------


      //-----------------------------------------------------
      ///\brief Check if a keyword already exists for the device
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\return true if the keyword exists, false if not
      //-----------------------------------------------------
      virtual bool keywordExists(const std::string& device, const std::string& keyword) const = 0;

      //-----------------------------------------------------
      ///\brief Declare a custom keyword
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\param    [in]    capacity           The capacity name (see yApi::CStandardCapacities for standard capacities, or use your own)
      ///\param    [in]    accessMode         The keyword access
      ///\param    [in]    type               The keyword type
      ///\param    [in]    units              The keyword units
      ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
      ///\return true if the keyword has been successfully created, false if already exist
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
      virtual bool declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, EKeywordAccessMode accessMode, EKeywordType type, const std::string & units = shared::CStringExtension::EmptyString, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) = 0;
      
      //-----------------------------------------------------
      ///\brief Declare a standard keyword
      ///\param    [in]    device             The device name owner of the keyword
      ///\param    [in]    keyword            The keyword name
      ///\param    [in]    capacity           The capacity name (see yApi::CStandardCapacities for standard capacities, or use your own)
      ///\param    [in]    details            The keyword details (JSON string, optional. Can be used to declare specific properties like min/max values)
      ///\return true if the keyword has been successfully created, false if already exist
      ///\throw shared::exception::CEmptyResult if creation failed
      //-----------------------------------------------------   
      virtual bool declareKeyword(const std::string& device, const std::string& keyword, const CStandardCapacity & capacity, const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) = 0;

      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Data recording
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Historize a new data values
      ///\param    [in]    device             The device name
      ///\param    [in]    keyword            The value name ("temp1", "temp2", "humidity", "batteryLevel", "rssi"...)
      ///\param    [in]    value              The capacity value
      ///\throw   shared::exception::CInvalidParameter if the device is not known
      ///\note Data are all recorded internally as string. The methods which doesn't take string just do a boost::lexical_cast.
      //-----------------------------------------------------     
      virtual void historizeData(const std::string & device, const std::string & keyword, const std::string & value) = 0;
      virtual void historizeData(const std::string & device, const std::string & keyword, bool value) = 0;
      virtual void historizeData(const std::string & device, const std::string & keyword, int value) = 0;
      virtual void historizeData(const std::string & device, const std::string & keyword, double value) = 0;
      
      //-----------------------------------------------------
      ///\brief Historize a new data (double) and round it with a specified number of digits
      ///\param    [in]    device             The device name
      ///\param    [in]    keyword            The value name ("temp1", "temp2", "humidity", "batteryLevel", "rssi"...)
      ///\param    [in]    value              The capacity value
      ///\param    [in]    numberOfdigits     The number of digits ("45.12345" with 2 digits => 45.12)
      ///\throw   shared::exception::CInvalidParameter if the device is not known
      ///\note Data are all recorded internally as string. The methods which doesn't take string just do a boost::lexical_cast.
      //-----------------------------------------------------     
      virtual void historizeData(const std::string & device, const std::string & keyword, double value, int numberOfdigits) = 0;
      
      
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin information
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Get the current plugin information (extracted from package.json file)
      ///\return The current plugin information
      //-----------------------------------------------------      
      virtual const shared::plugin::information::IInformation& getInformation() const = 0;


      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin configuration
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------

      //-----------------------------------------------------
      ///\brief Get the current plugin configuration
      ///\return The current plugin configuration
      //-----------------------------------------------------      
      virtual shared::CDataContainer getConfiguration() const = 0;


      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin event recorder
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      
      //-----------------------------------------------------
      ///\brief The plugin event record level
      //-----------------------------------------------------      
      typedef enum
      {
         kInfo,
         kError,
      }PluginEventSeverity;
      
      //-----------------------------------------------------
      ///\brief Record a plugin major event (recorded in Yadoms database)
      ///\param    [in]    severity           The message severity
      ///\param    [in]    message            The message
      //-----------------------------------------------------      
      virtual void recordPluginEvent(PluginEventSeverity severity, const std::string & message) = 0;



      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //--
      //-- Plugin information
      //--
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      //----------------------------------------------------------------------------------------------------------------
      
      //-----------------------------------------------------
      ///\brief Get the plugin binary path
      ///\return the plugin binary path
      //-----------------------------------------------------      
      virtual const boost::filesystem::path getPluginPath() const = 0;
   };
	
} } } // namespace shared::plugin::yadomsApi	
	
	
#pragma once
#include "ISmsDialerConfiguration.h"
#include "GammuPhoneConnection.h"
#include "IPhone.h"
#include <gammu.h>

//--------------------------------------------------------------
/// \brief	Use GAMMU library (http://wammu.eu/) to dial with phone
//--------------------------------------------------------------
class CGammuPhone : public IPhone
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CGammuPhone(const ISmsDialerConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CGammuPhone();

   // IPhone implementation
   virtual bool connect();
   virtual bool isConnected() const;
   virtual void send(boost::shared_ptr<ISms> sms);
   boost::shared_ptr<std::vector<ISms> > getIncomingSMS();
  // [END] IPhone implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                  Gammu error handler
   /// \param[in] gsmError       Gammu error value
   /// \param[in] errorMessage   Error message to pass to exception if gsmError != ERR_NONE
   /// \throw  CPhoneException if error
   //--------------------------------------------------------------
   void handleGammuError(GSM_Error gsmError, const std::string& errorMessage) const;

   //--------------------------------------------------------------
   /// \brief	                  Callback called by Gammu when SMS was sent
   /// \param[in] gsmContext     The Gammu context
   /// \param[in] status         SMS sending status
   /// \param[in] MessageReference  Reference on SMS sent
   /// \param[in] user_data      Used here to store this
   //--------------------------------------------------------------
   static void sendSmsCallback(GSM_StateMachine *gsmContext, int status, int MessageReference, void * user_data);

   //--------------------------------------------------------------
   /// \brief	                  Read and delete SMS from phone
   /// \param[in] deleteSms         true to delete the read message
   /// \return                   The list of received SMS
   //--------------------------------------------------------------
   boost::shared_ptr<std::vector<ISms> > readSms(bool deleteSms = true);

   //--------------------------------------------------------------
   /// \brief	                  Check if message is valid
   /// \param[in] gammuSms       The message
   /// \return                   true if valid
   //--------------------------------------------------------------
   bool isValidMessage(GSM_MultiSMSMessage* gammuSms) const;

   //--------------------------------------------------------------
   /// \brief	                  Check if message is multi-parted
   /// \param[in] gammuSms       The message
   /// \return                   true if multi-parted
   //--------------------------------------------------------------
   bool checkMultipart(GSM_MultiSMSMessage* gammuSms);

private:
   //--------------------------------------------------------------
   /// \brief	   The plugin instance configuration
   //--------------------------------------------------------------
   const ISmsDialerConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief	   Phone connection
   //--------------------------------------------------------------
   CGammuPhoneConnection m_connection;

   //--------------------------------------------------------------
   /// \brief	   SMS Send Gammu status
   //--------------------------------------------------------------
   GSM_Error m_smsSendStatus;
};




#pragma once
#include "ITypeInfo.h"
#include <shared/Export.h>
#include <shared/Field.hpp>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            namespace typeInfo
            {
               class YADOMS_SHARED_EXPORT CIntTypeInfo : public ITypeInfo
               {
               public:
                  static CIntTypeInfo Empty;

                  CIntTypeInfo();
                  CIntTypeInfo(int minValue,
                               int maxValue);
                  CIntTypeInfo(int minValue,
                               int maxValue,
                               int stepValue);
                  virtual ~CIntTypeInfo();

                  CIntTypeInfo& setMin(int minValue);
                  CIntTypeInfo& setMax(int maxValue);
                  CIntTypeInfo& setStep(int stepValue);

                  // ITypeInfo implementation 
                  CDataContainer serialize() const override;
                  // END ITypeInfo implementation 

               private:
                  CField<int> m_min;
                  CField<int> m_max;
                  CField<int> m_step;
               };
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization::typeInfo

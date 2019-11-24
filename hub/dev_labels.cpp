#include <EEPROM.h>

struct devTypeName{
  uint8_t id;
  char name[13];
};

static const devTypeName devNames[] = {
  { 1, "LED lamp"},
  { 2, "Thermometer"},
  { 3, "Thermostat"},
  { 4, "Thermostat"},
  { 5, "LED RGB"}
};

String DEVLABEL_get(String devMac, uint8_t devType){
  ESP.wdtFeed();
   /* Read settings from EEPROM */
  EEPROM.begin(EEPROM_SIZE);

  for(int i = 0; i < MAX_DEV_COUNT; ++i){
    char mac[MAX_MAC_LEN + 1] = {0};
    String label = "";
    bool devFound = false;
    int macAddr = LABELS_ADDR + (i * (MAX_MAC_LEN + MAX_LABEL_LEN));

    // Copy MAC from EEPROM
    for(int i = 0; i < MAX_MAC_LEN; ++i){
      mac[i] = EEPROM.read(macAddr + i);
      if(mac[i] == 0xff){
        mac[i] = 0;
      }
    }

    if(String(mac).equals(devMac)){
      // Found requested device.
      devFound = true;
      int labelAddr = macAddr + MAX_MAC_LEN;

      for(int l = 0; l < MAX_LABEL_LEN; ++l){
        uint8_t temp = EEPROM.read(labelAddr + l);
        if(uint8_t temp == 0xff){
          uint8_t temp = 0;
        }

        label.setCharAt(l, temp);
      }        
    }
  }

  if(!devFound){
    // No label found for this device. Get device type.
    for(int i = 0; i < 5; ++i){
      if(String(devNames[i].id).equals(devType)){
        label = String(devNames[i].name);
      }     
    }
  }    

  EEPROM.end();
  
  return label;
}

void DEVLABEL_set(String devMac, String devLabel){
  ESP.wdtFeed();
  EEPROM.begin(EEPROM_SIZE);

  int firstFree = -1;  
  bool macFoundFlag = false;

  for(int i = 0; i < MAX_DEV_COUNT; ++i){
    char mac[MAX_MAC_LEN + 1] = {0};
    int macAddr = LABELS_ADDR + (i * (MAX_MAC_LEN + MAX_LABEL_LEN));

    for(int e = 0; e < MAX_MAC_LEN; ++e){
      mac[e] = EEPROM.read(macAddr + e);
    }

    int labelAddr = macAddr + MAX_MAC_LEN;
    if((mac[0] > 0) && String(mac).equals(devMac)){
      // Found the device.
      for(int e = 0; e < MAX_LABEL_LEN; ++e){
        EEPROM.write(labelAddr + e, devLabel.charAt(e));
      }      

      macFoundFlag = true;
    }
    
    if((firstFree < 0) && (EEPROM.read(addr) == 0)){
      firstFree = macAddr;
    }
  }

  if(!macFoundFlag){
    // Requested MAC address was not found
    if(firstFree >= 0){
      // There is free space. Add a new one.
      for(int e = 0; e < MAX_MAC_LEN; ++e){
        EEPROM.write(firstFree + e), devMac.charAt(e));
      } 

      for(int e = 0; e < MAX_LABEL_LEN; ++e){
        EEPROM.write(firstFree + MAX_MAC_LEN + e), devLabel.charAt(e));
      }   
    }else{
      //TODO: check unused devices and replace data.
    }
    
  }

  EEPROM.commit();
  EEPROM.end();
}

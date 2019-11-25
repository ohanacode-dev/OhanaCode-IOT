#include <EEPROM.h>
#include "config.h"
#include "scan.h"

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

static void writeDevMac(int addr, String devMac){
  int len = devMac.length();
  if(len > MAX_MAC_LEN){
    len = MAX_MAC_LEN;
  }
  
  for(int i = 0; i < MAX_MAC_LEN; ++i){
    if(i < len){
      EEPROM.write(addr + i, devMac.charAt(i));
    }else{
      EEPROM.write(addr + i, 0);
    }    
  }
}

static void writeDevLabel(int addr, String devLabel){
  int len = devLabel.length();
  if(len > MAX_LABEL_LEN){
    len = MAX_LABEL_LEN;
  }
  
  for(int i = 0; i < MAX_LABEL_LEN; ++i){
    if(i < len){
      EEPROM.write(addr + i, devLabel.charAt(i));
    }else{
      EEPROM.write(addr + i, 0);
    }
  }
}

static String readDevMac(int addr){
  char mac[MAX_MAC_LEN + 1] = {0};
  
  for(int i = 0; i < MAX_MAC_LEN; ++i){
    mac[i] = EEPROM.read(addr + i);
    
    if(mac[i] == 0xff){
      mac[i] = 0;
      break;
    }
  }

  return String(mac);
}

static String readDevLabel(int addr){
  char label[MAX_LABEL_LEN + 1] = {0};
  
  for(int i = 0; i < MAX_LABEL_LEN; ++i){
    label[i] = EEPROM.read(addr + i);
    
    if(label[i] == 0xff){
      label[i] = 0;
      break;
    }
  }

  return String(label);
}

int getUnusedDevAddr(void){
 
  for(int i = 0; i < MAX_DEV_COUNT; ++i){
    int macAddr = LABELS_ADDR + (i * (MAX_MAC_LEN + MAX_LABEL_LEN));
    String mac = readDevMac(macAddr);  

    if(!SCAN_isDevPresent(mac)){
      return macAddr;
    }
  }

  return -1;
}

String DEVLABEL_get(String devMac, uint8_t devType){
  ESP.wdtFeed();
   /* Read settings from EEPROM */
  EEPROM.begin(EEPROM_SIZE);

  String label;
  bool devFound = false;

  for(int i = 0; i < MAX_DEV_COUNT; ++i){
    int macAddr = LABELS_ADDR + (i * (MAX_MAC_LEN + MAX_LABEL_LEN));
    String mac = readDevMac(macAddr);    
    
    if(mac.equals(devMac)){
      // Found requested device.      
      devFound = true;
      label = readDevLabel(macAddr + MAX_MAC_LEN);
  
      break;
    }
  }

  if(!devFound){
    // No label found for this device. Get device type.
    for(int i = 0; i < 5; ++i){
      if(devNames[i].id == devType){
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
    int macAddr = LABELS_ADDR + (i * (MAX_MAC_LEN + MAX_LABEL_LEN));
    String mac = readDevMac(macAddr);  
          
    if(mac.equals(devMac)){
      // Found the device.
      int labelAddr = macAddr + MAX_MAC_LEN;

      writeDevLabel(labelAddr, devLabel);

      macFoundFlag = true;
      break;
    }
    
    if((firstFree < 0) && ((EEPROM.read(macAddr) == 0) || (EEPROM.read(macAddr) == 0xff))){
      firstFree = macAddr;
    }
  }

  if(!macFoundFlag){
    // Requested MAC address was not found
    if(firstFree >= 0){
      // There is free space. Add a new one.
      writeDevMac(firstFree, devMac);
      writeDevLabel(firstFree + MAX_MAC_LEN, devLabel);
      
    }else{
      // No free space. Find an unused device and replace data.
      int unusedAddr = getUnusedDevAddr();
      if(unusedAddr > 0){
        writeDevMac(unusedAddr, devMac);
        writeDevLabel(unusedAddr + MAX_MAC_LEN, devLabel);
      }
    }
    
  }

  EEPROM.commit();
  EEPROM.end();
}

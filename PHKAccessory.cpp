//
//  PHKAccessory.c
//  Workbench
//
//  Created by Wai Man Chan on 9/27/14.
//
//

#include "PHKAccessory.h"
#include "Configuration.h"


static const char hapJsonType[] = "application/hap+json";
static const char pairingTlv8Type[] = "application/pairing+tlv8";


void *announce(void *info) {
    broadcastInfo *_info = (broadcastInfo *)info;
    void *sender = _info->sender;
    char *desc = _info->desc;
    
    char reply[1024];
    int len = snprintf(reply, 1024, "EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %lu\r\n\r\n%s", strlen(desc), desc);
    
#if HomeKitLog == 1 && HomeKitReplyHeaderLog==1
    printf("%s\n", reply);
#endif
    
    broadcastMessage(sender, reply, len);
    
    delete[] desc;
    delete info;

}

//Wrap to JSON
inline std::string wrap(const char *str) { return (std::string)"\"" + str + "\""; }

//Value String
inline std::string attribute(unsigned int type, unsigned short acclaim, int p, std::string value) 
{
	std::string result;
    if (p & permission_read) {
        result += wrap("value")+":";
        result += value;
        result += ",";
    }
    
	result += wrap("perms") + ":";
    result += "[";
	if (p & permission_read) result += wrap("pr") + ",";
	if (p & permission_write) result += wrap("pw") + ",";
	if (p & permission_notify) result += wrap("ev") + ",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    char tempStr[4];
    snprintf(tempStr, 4, "%X", type);
	result += wrap("type") + ":" + wrap(tempStr);
    result += ",";
    
	snprintf(tempStr, 4, "%hd", acclaim);
	result += wrap("iid") + ":" + tempStr;
    result += ",";
    
    result += "\"format\":\"bool\"";
    
	return "{" + result + "}";
}

inline std::string attribute(unsigned int type, unsigned short acclaim, int p, std::string value, int minVal, int maxVal, int step, unit valueUnit)
{
	std::string result;
    char tempStr[16];
    
    if (p & permission_read) {
		result += wrap("value") + ":" + value;
        result += ",";
    }
    
    snprintf(tempStr, 16, "%d", minVal);
    if (minVal != INT32_MIN)
        result += wrap("minValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%d", maxVal);
    if (maxVal != INT32_MAX)
        result += wrap("maxValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%d", step);
    if (step > 0)
        result += wrap("minStep")+":"+tempStr+",";
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    snprintf(tempStr, 16, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 16, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    switch (valueUnit) {
        case unit_arcDegree:
            result += wrap("unit")+":"+wrap("arcdegrees")+",";
            break;
        case unit_celsius:
            result += wrap("unit")+":"+wrap("celsius")+",";
            break;
        case unit_percentage:
            result += wrap("unit")+":"+wrap("percentage")+",";
            break;
    }
    
    result += "\"format\":\"int\"";
    
    return "{"+result+"}";
}

inline std::string attribute(unsigned int type, unsigned short acclaim, int p, std::string value, float minVal, float maxVal, float step, unit valueUnit) {
	std::string result;
    char tempStr[16];
    
    if (p & permission_read) {
        result += wrap("value")+":"+value;
        result += ",";
    }
    
    snprintf(tempStr, 16, "%f", minVal);
    if (minVal != INT32_MIN)
        result += wrap("minValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%f", maxVal);
    if (maxVal != INT32_MAX)
        result += wrap("maxValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%f", step);
    if (step > 0)
        result += wrap("minStep")+":"+tempStr+",";
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    snprintf(tempStr, 16, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 16, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    switch (valueUnit) {
        case unit_arcDegree:
            result += wrap("unit")+":"+wrap("arcdegrees")+",";
            break;
        case unit_celsius:
            result += wrap("unit")+":"+wrap("celsius")+",";
            break;
        case unit_percentage:
            result += wrap("unit")+":"+wrap("percentage")+",";
            break;
    }
    
    result += "\"format\":\"float\"";
    
    return "{"+result+"}";
}
//Raw value
inline std::string attribute(unsigned int type, unsigned short acclaim, int p, bool value) 
{
	std::string result;
    if (p & permission_read) {
        result += wrap("value")+":";
        if (value) result += "true";
        else result += "false";
        result += ",";
    }
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    char tempStr[4];
    snprintf(tempStr, 4, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 4, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    result += "\"format\":\"bool\"";
    
    return "{"+result+"}";
}

inline std::string attribute(unsigned int type, unsigned short acclaim, int p, int value, int minVal, int maxVal, int step, unit valueUnit) 
{
	std::string result;
    char tempStr[16];
    
    snprintf(tempStr, 16, "%d", value);
    
    if (p & permission_read) {
        result += wrap("value")+":"+tempStr;
        result += ",";
    }
    
    snprintf(tempStr, 16, "%d", minVal);
    if (minVal != INT32_MIN)
        result += wrap("minValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%d", maxVal);
    if (maxVal != INT32_MAX)
        result += wrap("maxValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%d", step);
    if (step > 0)
        result += wrap("minStep")+":"+tempStr+",";
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    snprintf(tempStr, 16, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 16, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    switch (valueUnit) {
        case unit_arcDegree:
            result += wrap("unit")+":"+wrap("arcdegrees")+",";
            break;
        case unit_celsius:
            result += wrap("unit")+":"+wrap("celsius")+",";
            break;
        case unit_percentage:
            result += wrap("unit")+":"+wrap("percentage")+",";
            break;
    }
    
    result += "\"format\":\"int\"";
    
    return "{"+result+"}";
}

inline std::string attribute(unsigned int type, unsigned short acclaim, int p, float value, float minVal, float maxVal, float step, unit valueUnit) 
{
	std::string result;
    char tempStr[16];
    
    snprintf(tempStr, 16, "%f", value);
    
    if (p & permission_read) {
        result += wrap("value")+":"+tempStr;
        result += ",";
    }
    
    snprintf(tempStr, 16, "%f", minVal);
    if (minVal != INT32_MIN)
        result += wrap("minValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%f", maxVal);
    if (maxVal != INT32_MAX)
        result += wrap("maxValue")+":"+tempStr+",";
    
    snprintf(tempStr, 16, "%f", step);
    if (step > 0)
        result += wrap("minStep")+":"+tempStr+",";
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    snprintf(tempStr, 16, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 16, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    switch (valueUnit) {
        case unit_arcDegree:
            result += wrap("unit")+":"+wrap("arcdegrees")+",";
            break;
        case unit_celsius:
            result += wrap("unit")+":"+wrap("celsius")+",";
            break;
        case unit_percentage:
            result += wrap("unit")+":"+wrap("percentage")+",";
            break;
    }
    
    result += "\"format\":\"float\"";
    
    return "{"+result+"}";
}

inline std::string attribute(unsigned int type, unsigned short acclaim, int p, std::string value, unsigned short len) 
{
	std::string result;
    char tempStr[4];
    
    if (p & permission_read) {
        result += wrap("value")+":"+value.c_str();
        result += ",";
    }
    
    result += wrap("perms")+":";
    result += "[";
    if (p & permission_read) result += wrap("pr")+",";
    if (p & permission_write) result += wrap("pw")+",";
    if (p & permission_notify) result += wrap("ev")+",";
    result = result.substr(0, result.size()-1);
    result += "]";
    result += ",";
    
    snprintf(tempStr, 4, "%X", type);
    result += wrap("type")+":"+wrap(tempStr);
    result += ",";
    
    snprintf(tempStr, 4, "%hd", acclaim);
    result += wrap("iid")+":"+tempStr;
    result += ",";
    
    if (len > 0) {
        snprintf(tempStr, 4, "%hd", len);
        result += wrap("maxLen")+":"+tempStr;
        result += ",";
    }
    
    result += "\"format\":\"string\"";
    
	return "{" + result + "}";
}

inline std::string arrayWrap(std::string *s, unsigned short len) 
{
	std::string result;
    
    result += "[";
    
    for (int i = 0; i < len; i++) {
        result += s[i]+",";
    }
    result = result.substr(0, result.size()-1);
    
    result += "]";
    
    return result;
}

inline std::string dictionaryWrap(std::string *key, std::string *value, unsigned short len) 
{
	std::string result;
    
    result += "{";
    
    for (int i = 0; i < len; i++) {
        result += wrap(key[i].c_str())+":"+value[i]+",";
    }
    result = result.substr(0, result.size()-1);
    
    result += "}";
    
    return result;
}

void characteristics::notify() 
{
    char *broadcastTemp = new char[1024];
	snprintf(broadcastTemp, 1024, "{\"characteristics\":[{\"aid\": %d, \"iid\": %d, \"value\": %s}]}", accessory->aid, iid, value(NULL).c_str());

    broadcastInfo * info = new broadcastInfo;
    info->sender = this;
    info->desc = broadcastTemp;

    pthread_t thread;
    pthread_create(&thread, NULL, announce, info);
	pthread_detach(thread);
}

std::string boolCharacteristics::describe(connectionInfo *sender) {
    return attribute(type, iid, premission, value(sender));
}

std::string floatCharacteristics::describe(connectionInfo *sender) {
    return attribute(type, iid, premission, value(sender), _minVal, _maxVal, _step, _unit);
}

std::string intCharacteristics::describe(connectionInfo *sender) {
    return attribute(type, iid, premission, value(sender), _minVal, _maxVal, _step, _unit);
}

std::string stringCharacteristics::describe(connectionInfo *sender) {
    return attribute(type, iid, premission, value(sender), maxLen);
}

std::string Service::describe(connectionInfo *sender) {
	std::string keys[3] = {"iid", "type", "characteristics"};
	std::string values[3];
    {
        char temp[8];
        snprintf(temp, 8, "%d", serviceID);
        values[0] = temp;
    }
    {
        char temp[8];
        snprintf(temp, 8, "\"%X\"", uuid);
        values[1] = temp;
    }
    {
        int no = numberOfCharacteristics();
		std::string *chars = new std::string[no];
        for (int i = 0; i < no; i++) {
            chars[i] = _characteristics[i]->describe(sender);
        }
        values[2] = arrayWrap(chars, no);
        delete [] chars;
    }
    return dictionaryWrap(keys, values, 3);
}

std::string Accessory::describe(connectionInfo *sender) {
	std::string keys[2];
	std::string values[2];
    
    {
        keys[0] = "aid";
        char temp[8];
        sprintf(temp, "%d", aid);
        values[0] = temp;
    }
    
    {
        //Form services list
        int noOfService = numberOfService();
		std::string *services = new std::string[noOfService];
        for (int i = 0; i < noOfService; i++) {
            services[i] = _services[i]->describe(sender);
        }
        keys[1] = "services";
        values[1] = arrayWrap(services, noOfService);
        delete [] services;
    }
    
	std::string result = dictionaryWrap(keys, values, 2);
    return result;
}

std::string AccessorySet::describe(connectionInfo *sender) {
    int numberOfAcc = numberOfAccessory();
	std::string *desc = new std::string[numberOfAcc];
    for (int i = 0; i < numberOfAcc; i++) {
        desc[i] = _accessories[i]->describe(sender);
    }

	std::string result = arrayWrap(desc, numberOfAcc);
    delete [] desc;
	std::string key = "accessories";
    
	result = dictionaryWrap(&key, &result, 1);
    return result;
}

void updateValueFromDeviceEnd(characteristics *c, int aid, int iid, std::string value) {
    c->setValue(value);
    char *broadcastTemp = new char[1024];
    snprintf(broadcastTemp, 1024, "{\"characteristics\":[{\"aid\":%d,\"iid\":%d,\"value\":%s}]}", aid, iid, value.c_str());

    broadcastInfo * info = new broadcastInfo;
    info->sender = c;
    info->desc = broadcastTemp;

    pthread_t thread;
    pthread_create(&thread, NULL, announce, info);
	pthread_detach(thread);
}

void handleAccessory(const char *request, unsigned int requestLen, char **reply, unsigned int *replyLen, connectionInfo *sender) {
#if HomeKitLog == 1
    printf("Receive request: %s\n", request);
#endif
    int index = 5;
    char method[5];
    {
        //Read method
        method[4] = 0;
        bcopy(request, method, 4);
        if (method[3] == ' ') {
            method[3] = 0;
            index = 4;
        }
    }
    
    char path[1024];
    int i;
    for (i = 0; i < 1024 && request[index] != ' '; i++, index++) {
        path[i] = request[index];
    }
    path[i] = 0;
#if HomeKitLog == 1
    printf("Path: %s\n", path);
#endif
    
    const char *dataPtr = request;
    while (true) {
        dataPtr = &dataPtr[1];
        if (dataPtr[0] == '\r' && dataPtr[1] == '\n' && dataPtr[2] == '\r' && dataPtr[3] == '\n') break;
    }
    
    dataPtr += 4;
    
    char *replyData = NULL;  unsigned short replyDataLen = 0;
    
    int statusCode = 0;
    
    const char *protocol = "HTTP/1.1";
    const char *returnType = hapJsonType;
    
    if (strcmp(path, "/accessories") == 0) {
        //Publish the characterists of the accessories
#if HomeKitLog == 1
        printf("Ask for accessories info\n");
#endif
        statusCode = 200;
		std::string desc = AccessorySet::getInstance().describe(sender);
        replyDataLen = desc.length();
        replyData = new char[replyDataLen+1];
        bcopy(desc.c_str(), replyData, replyDataLen);
        replyData[replyDataLen] = 0;
    } else if (strcmp(path, "/pairings") == 0) {
        PHKNetworkMessage msg(request);
        statusCode = 200;
    #if HomeKitLog == 1
        printf("%d\n", *msg.data.dataPtrForIndex(0));
    #endif
        if (*msg.data.dataPtrForIndex(0) == 3) {
            //Pairing with new user

            printf("Add new user\n");

            PHKKeyRecord controllerRec;
            bcopy(msg.data.dataPtrForIndex(3), controllerRec.publicKey, 32);
            bcopy(msg.data.dataPtrForIndex(1), controllerRec.controllerID, 36);
            addControllerKey(controllerRec);
            PHKNetworkMessageDataRecord drec;
            drec.activate = true; drec.data = new char[1]; *drec.data = 2;
            drec.index = 6; drec.length = 1;
            PHKNetworkMessageData data;
            data.addRecord(drec);
            data.rawData((const char **)&replyData, &replyDataLen);
            returnType = pairingTlv8Type;
            statusCode = 200;
        } else {

            printf("Delete user");

            PHKKeyRecord controllerRec;
            bcopy(msg.data.dataPtrForIndex(1), controllerRec.controllerID, 36);
            removeControllerKey(controllerRec);
            PHKNetworkMessageDataRecord drec;
            drec.activate = true; drec.data = new char[1]; *drec.data = 2;
            drec.index = 6; drec.length = 1;
            PHKNetworkMessageData data;
            data.addRecord(drec);
            data.rawData((const char **)&replyData, &replyDataLen);
            returnType = pairingTlv8Type;
            statusCode = 200;
        }
        //Pairing status change, so update
        updatePairable();
    } else if (strncmp(path, "/characteristics", 16) == 0){
        pthread_mutex_lock(&AccessorySet::getInstance().accessoryMutex);
        printf("Characteristics\n");
        if (strncmp(method, "GET", 3) == 0) {
            //Read characteristics
            int aid = 0;    int iid = 0;
            
            char indexBuffer[1000];
            sscanf(path, "/characteristics?id=%[^\n]", indexBuffer);
            
            
            printf("Get characteristics %s with len %d\n", indexBuffer, strlen(indexBuffer));
            
            statusCode = 404;
            
			std::string result = "[";
            
            while (strlen(indexBuffer) > 0) {
                
                printf("Get characteristics %s with len %d\n", indexBuffer, strlen(indexBuffer));
                
                char temp[1000];
                //Initial the temp
                temp[0] = 0;
                sscanf(indexBuffer, "%d.%d%[^\n]", &aid, &iid, temp);
                printf("Get temp %s with len %d\n", temp, strlen(temp));
                strncpy(indexBuffer, temp, 1000);
                printf("Get characteristics %s with len %d\n", indexBuffer, strlen(indexBuffer));
                //Trim comma
                if (indexBuffer[0] == ',') {
                    indexBuffer[0] = '0';
                }
                
                Accessory *a = AccessorySet::getInstance().accessoryAtIndex(aid);
                if (a != NULL) {
                    characteristics *c = a->characteristicsAtIndex(iid);
                    if (c != NULL) {
#if HomeKitLog == 1
                        printf("Ask for one characteristics: %d . %d\n", aid, iid);
#endif

						std::string s[3] = {std::to_string(aid), std::to_string(iid), c->value(sender)};
						std::string k[3] = {"aid", "iid", "value"};
                        if (result.length() != 1) {
                            result += ",";
                        }
                        
						std::string _result = dictionaryWrap(k, s, 3);
                        result += _result;
                        
                    }
                    
                }
            }
            
            result += "]";
            
			std::string d = "characteristics";
            result = dictionaryWrap(&d, &result, 1);
            
            replyDataLen = result.length();
            replyData = new char[replyDataLen+1];
            replyData[replyDataLen] = 0;
            bcopy(result.c_str(), replyData, replyDataLen);
            statusCode = 200;
            
        } else if (strncmp(method, "PUT", 3) == 0) {
            //Change characteristics
            printf("PUT characteristics: \n");
            
            char characteristicsBuffer[1000];
            sscanf(dataPtr, "{\"characteristics\":[{%[^]]s}", characteristicsBuffer);
            
            char *buffer2 = characteristicsBuffer;
            while (strlen(buffer2) && statusCode != 400) {
                bool reachLast = false; bool updateNotify = false;
                char *buffer1;
                buffer1 = strtok_r(buffer2, "}", &buffer2);
                if (*buffer2 != 0) buffer2+=2;
                
                int aid = 0;    int iid = 0; char value[16];
                int result = sscanf(buffer1, "\"aid\":%d,\"iid\":%d,\"value\":%s", &aid, &iid, value);
                if (result == 2) {
                    sscanf(buffer1, "\"aid\":%d,\"iid\":%d,\"ev\":%s", &aid, &iid, value);
                    updateNotify = true;
                } else if (result == 0) {
                    sscanf(buffer1, "\"remote\":true,\"value\":%[^,],\"aid\":%d,\"iid\":%d", value, &aid, &iid);
                    if (result == 2) {
                        sscanf(buffer1, "\"remote\":true,\"aid\":%d,\"iid\":%d,\"ev\":%s", &aid, &iid, value);
                        updateNotify = true;
                    }
                    sender->relay = true;
                }
                printf("%d . %d\n",aid, iid);
                
                Accessory *a = AccessorySet::getInstance().accessoryAtIndex(aid);
                if (a==NULL) {
                    statusCode = 400;
                } else {
                    characteristics *c = a->characteristicsAtIndex(iid);
                    
                    if (updateNotify) {
#if HomeKitLog == 1
                        printf("Ask to notify one characteristics: %d . %d -> %s\n", aid, iid, value);
#endif
                        if (c==NULL) {
                            statusCode = 400;
                        } else {
                            if (c->notifiable()) {
                                if (strncmp(value, "1", 1)==0 || strncmp(value, "true", 4) == 0)
                                    sender->addNotify(c, aid, iid);
                                else
                                    sender->removeNotify(c);
                                
                                statusCode = 204;
                            } else {
                                statusCode = 400;
                            }
                        }
                    } else {
#if HomeKitLog == 1
                        printf("Ask to change one characteristics: %d . %d -> %s\n", aid, iid, value);
#endif
                        if (c==NULL) {
                            statusCode = 400;
                        } else {
                            if (c->writable()) {
                                c->setValue(value, sender);
                                
                                char *broadcastTemp = new char[1024];
                                snprintf(broadcastTemp, 1024, "{\"characteristics\":[{%s}]}", buffer1);
                                broadcastInfo * info = new broadcastInfo;
                                info->sender = c;
                                info->desc = broadcastTemp;
                                pthread_t thread;
                                pthread_create(&thread, NULL, announce, info);
								pthread_detach(thread);
                                
                                statusCode = 204;
                                
                            } else {
                                statusCode = 400;
                            }
                        }
                    }
                    
                }
                
            }
            
        } else {
            return;
        }
        pthread_mutex_unlock(&AccessorySet::getInstance().accessoryMutex);
    } else {
        //Error
#if HomeKitLog == 1
        printf("Ask for something I don't know\n");
#endif
        printf("%s\n", request);
        printf("%s", path);
        statusCode = 404;
    }
    
    //Calculate the length of header
    char * tmp = new char[256];
    bzero(tmp, 256);
    int len = snprintf(tmp, 256, "%s %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", protocol, statusCode, returnType, replyDataLen);
    delete [] tmp;
    
    //replyLen should omit the '\0'.
    (*replyLen) = len+replyDataLen;
    //reply should add '\0', or the printf is incorrect
    *reply = new char[*replyLen + 1];
    bzero(*reply, *replyLen + 1);
    snprintf(*reply, len + 1, "%s %d OK\r\nContent-Type: %s\r\nContent-Length: %u\r\n\r\n", protocol, statusCode, returnType, replyDataLen);
    
    if (replyData) {
        bcopy(replyData, &(*reply)[len], replyDataLen);
        delete [] replyData;
    }
    
#if HomeKitLog == 1 && HomeKitReplyHeaderLog==1
    printf("Reply: %s\n", *reply);
#endif
    
}

void addInfoServiceToAccessory(Accessory *acc, std::string accName, std::string manufactuerName, std::string modelName, 
	std::string serialNumber, identifyFunction identifyCallback) 
{
    Service *infoService = new Service(serviceType_accessoryInfo);
    acc->addService(infoService);
    
    stringCharacteristics *accNameCha = new stringCharacteristics(charType_serviceName, permission_read, 0);
    accNameCha->characteristics::setValue(accName);
    acc->addCharacteristics(infoService, accNameCha);
    
    stringCharacteristics *manNameCha = new stringCharacteristics(charType_manufactuer, permission_read, 0);
    manNameCha->characteristics::setValue(manufactuerName);
    acc->addCharacteristics(infoService, manNameCha);
    
    stringCharacteristics *modelNameCha = new stringCharacteristics(charType_modelName, permission_read, 0);
    modelNameCha->characteristics::setValue(modelName);
    acc->addCharacteristics(infoService, modelNameCha);
    
    stringCharacteristics *serialNameCha = new stringCharacteristics(charType_serialNumber, permission_read, 0);
    serialNameCha->characteristics::setValue(serialNumber);
    acc->addCharacteristics(infoService, serialNameCha);
    
    boolCharacteristics *identify = new boolCharacteristics(charType_identify, permission_write);
    identify->characteristics::setValue("false");
    identify->valueChangeFunctionCall = identifyCallback;
    acc->addCharacteristics(infoService, identify);
}

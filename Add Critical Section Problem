#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 3
#define MAX_RESOURCES 3
#define MAX_NAME_LEN 20

typedef enum{READ=1,WRITE=2,EXECUTE=4}Permission;

typedef struct{char name[MAX_NAME_LEN];}User;
typedef struct{char name[MAX_NAME_LEN];}Resource;

typedef struct{char username[MAX_NAME_LEN];int perms;}ACLEntry;
typedef struct{Resource res;ACLEntry entries[MAX_USERS];int aclCount;}ACLControlledResource;

typedef struct{char resourceName[MAX_NAME_LEN];int perms;}Capability;
typedef struct{User user;Capability caps[MAX_RESOURCES];int capCount;}CapabilityUser;

void printPermissions(int perm){
    int first=1;
    if(perm&READ){printf("Read");first=0;}
    if(perm&WRITE){if(!first)printf(",");printf("Write");first=0;}
    if(perm&EXECUTE){if(!first)printf(",");printf("Execute");first=0;}
    if(first)printf("None");
}

int hasPermission(int userPerm,int requiredPerm){
    return (userPerm & requiredPerm)==requiredPerm;
}

void checkACLAccess(ACLControlledResource *res,const char *userName,int perm){
    for(int i=0;i<res->aclCount;i++){
        if(strcmp(res->entries[i].username,userName)==0){
            int userPerm=res->entries[i].perms;
            if(hasPermission(userPerm,perm)){
                printf("ACL Check: User %s requests %d on %s: Access GRANTED\n",userName,perm,res->res.name);
            }else{
                printf("ACL Check: User %s requests %d on %s: Access DENIED\n",userName,perm,res->res.name);
            }
            return;
        }
    }
    printf("ACL Check: User %s has NO entry for resource %s: Access DENIED\n",userName,res->res.name);
}

void checkCapabilityAccess(CapabilityUser *user,const char *resourceName,int perm){
    for(int i=0;i<user->capCount;i++){
        if(strcmp(user->caps[i].resourceName,resourceName)==0){
            int userPerm=user->caps[i].perms;
            if(hasPermission(userPerm,perm)){
                printf("Capability Check: User %s requests %d on %s: Access GRANTED\n",user->user.name,perm,resourceName);
            }else{
                printf("Capability Check: User %s requests %d on %s: Access DENIED\n",user->user.name,perm,resourceName);
            }
            return;
        }
    }
    printf("Capability Check: User %s has NO capability for resource %s: Access DENIED\n",user->user.name,resourceName);
}

int main(){
    User users[MAX_USERS]={{"Alice"},{"Bob"},{"Charlie"}};
    Resource resources[MAX_RESOURCES]={{"File1"},{"File2"},{"File3"}};

    ACLControlledResource aclRes[MAX_RESOURCES];
    for(int i=0;i<MAX_RESOURCES;i++){
        strncpy(aclRes[i].res.name,resources[i].name,MAX_NAME_LEN-1);
        aclRes[i].res.name[MAX_NAME_LEN-1]='\0';
        aclRes[i].aclCount=0;
    }

    CapabilityUser capUsers[MAX_USERS];
    for(int i=0;i<MAX_USERS;i++){
        capUsers[i].user=users[i];
        capUsers[i].capCount=0;
    }

    //ACL Setup
    strncpy(aclRes[0].entries[0].username,"Alice",MAX_NAME_LEN);
    aclRes[0].entries[0].perms=READ|WRITE;
    strncpy(aclRes[0].entries[1].username,"Bob",MAX_NAME_LEN);
    aclRes[0].entries[1].perms=READ;
    aclRes[0].aclCount=2;

    strncpy(aclRes[1].entries[0].username,"Charlie",MAX_NAME_LEN);
    aclRes[1].entries[0].perms=READ|EXECUTE;
    aclRes[1].aclCount=1;

    strncpy(aclRes[2].entries[0].username,"Alice",MAX_NAME_LEN);
    aclRes[2].entries[0].perms=EXECUTE;
    aclRes[2].aclCount=1;

    //Capability Setup
    strncpy(capUsers[0].caps[0].resourceName,"File1",MAX_NAME_LEN);
    capUsers[0].caps[0].perms=READ|WRITE;
    capUsers[0].capCount++;

    strncpy(capUsers[1].caps[0].resourceName,"File1",MAX_NAME_LEN);
    capUsers[1].caps[0].perms=READ;
    capUsers[1].capCount++;

    strncpy(capUsers[2].caps[0].resourceName,"File2",MAX_NAME_LEN);
    capUsers[2].caps[0].perms=READ|EXECUTE;
    capUsers[2].capCount++;

    //Test ACL
    checkACLAccess(&aclRes[0],"Alice",READ);
    checkACLAccess(&aclRes[0],"Bob",WRITE);
    checkACLAccess(&aclRes[1],"Charlie",EXECUTE);
    checkACLAccess(&aclRes[2],"Alice",EXECUTE);
    checkACLAccess(&aclRes[2],"Bob",READ);

    //Test Capability
    checkCapabilityAccess(&capUsers[0],"File1",WRITE);
    checkCapabilityAccess(&capUsers[1],"File1",WRITE);
    checkCapabilityAccess(&capUsers[2],"File2",EXECUTE);
    checkCapabilityAccess(&capUsers[1],"File3",READ);

    return 0;
}

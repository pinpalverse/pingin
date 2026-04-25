#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { INT, FLOAT, BOOL, TEXT, NONE } CONF_V_TYPE;

typedef struct {
  char *k;
  char *v;
  CONF_V_TYPE v_type;
} ConfKV;

typedef struct {
  char *filename;
  int size;
  int columns;
  ConfKV **values;
} Conf;

int parse(const char *filename, Conf* cfg) {

  FILE *conf = fopen(filename, "r");
  if (!conf) {
    char s[200];
    sprintf(s,"Line %d: fopen", __LINE__);
    perror(s);
    return 1;
  }
  fseek(conf, 0, SEEK_END);
  int fp = ftell(conf);
  fseek(conf, 0, SEEK_SET);

  char *data = (char *)malloc(fp + sizeof(char));

  fread(data, fp, fp - 1, conf);
  cfg->filename = (char*)malloc(strlen(filename)+sizeof(char));
  strcpy(cfg->filename, filename);
  cfg->size = fp;


  char* tk = strtok(data,"\n");

  int i = 0;
  while(tk != NULL){
    if(tk[0] == '#'){goto NEWTK;}
    char* s = strchr(tk,(int)'=');
    int ix = strcspn(tk,"="); 
    ConfKV c;
    c.k = (char*)malloc((ix+1) * sizeof(char));
    c.v = (char*)malloc(strlen(&s[1])+ sizeof(char));
    c.v_type = NONE;
    if(cfg->values == NULL){
      cfg->values = (ConfKV**) reallocarray(NULL, 1, sizeof(c));
    }else{
      cfg->values = (ConfKV**) reallocarray(cfg->values, i, sizeof(c));
    }
    cfg->values[i] = (ConfKV*)malloc(sizeof(c));
    strncpy(c.k, &tk[0], ix);
    if(s[1] == '"'){
      strncpy(c.v, &s[2], strlen(s)-3+1);
      cfg->values[i]->v_type = TEXT;
    }else{
      strcpy(c.v, &s[1]);
    }
    
    memcpy(cfg->values[i], &c, sizeof(c));
    i++;
    
    NEWTK:
    tk = strtok(NULL,"\n");
  }
  cfg->columns = i;
  
  free(data);
  return 0;
}
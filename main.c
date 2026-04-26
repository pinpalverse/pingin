#include <stdio.h>
#include "conf_parser.h"


int main(){


    Conf conf;
    parse("main.pconf", &conf);
    
    for(int i = 0; i < conf.columns; i++){
        switch (conf.values[i]->v_type) {
            case NONE:
                printf("NONE ");
                break;
            case INT:
                printf("INT ");
                break;
            case TEXT:
                printf("TEXT ");
                break;
            case FLOAT:
                printf("FLOAT ");
                break;
            case BOOL:
                printf("BOOL ");
                break;
            default:
                break;
        }
        printf("%s : %s\n",conf.values[i]->k, conf.values[i]->v);
        free(conf.values[i]->k);
        free(conf.values[i]->v);
        free(conf.values[i]);
    }
    return 0;
}
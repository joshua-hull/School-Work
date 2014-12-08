typedef struct tiled {
    int magic;
    double width;
    double height;
    pixel_t color;
    intensity_t diffuse;
    intensity_t reflective;
    pixel_t color2;
    intensity_t diffuse2;
    intensity_t reflective2;
    
} tiled_t;

entity_t *newTiled(char *enttype, int code);
void loadTiled(FILE *inFP, entity_t *ent, char *token);
void completeTiled(scene_t *scene, entity_t *ent);
void dumpTiled(FILE *outFP, entity_t *ent);
int hitTiled(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit);

#define TILED_T 86420

typedef struct ring {
    int magic;
    double radius1;
    double radius2;
} ring_t;

entity_t *newRing(char *enttype, int code);
void loadRing(FILE *inFP, entity_t *ent, char *token);
void completeRing(scene_t *scene, entity_t *ent);
void dumpRing(FILE *outFP, entity_t *ent);
int hitRing(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit);

#define RING_T 97531

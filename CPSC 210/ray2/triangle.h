typedef struct triangle {
    int magic;
    double length1;
    double length2;
    edge_t edges[3];
    void *triDerived;
} triangle_t;

entity_t *newTriangle(char *enttype, int code);
void loadTriangle(FILE *inFP, entity_t *ent, char *token);
void completeTriangle(scene_t *scene, entity_t *ent);
void dumpTriangle(FILE *outFP, entity_t *ent);
int hitTriangle(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit);

#define TRIANGLE_T 1234097

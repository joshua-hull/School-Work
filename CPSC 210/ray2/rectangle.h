typedef struct rectangle {
    int magic;
    double width;
    double height;
    edge_t edges[4];
    void *rectDerived;
} rectangle_t;

entity_t *newRectangle(char *enttype, int code);
void loadRectangle(FILE *inFP, entity_t *ent, char *token);
void completeRectangle(scene_t *scene, entity_t *ent);
void dumpRectangle(FILE *outFP, entity_t *ent);
int hitRectangle(entity_t *ent, point_t base, vector_t dir, hitinfo_t *hit);

#define RECTANGLE_T 7904321

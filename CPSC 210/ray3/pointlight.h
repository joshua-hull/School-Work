typedef struct lightType {
    int         magic;          // pointlight_t magic number
    pixel_t     color;          // light's rgb color
    double      brightness;     // intensity level
    point_t     center;         // Center of light
    void        *lightDerived;  // Derived object data
}   pointlight_t;

entity_t *newPointLight(char *enttype,int code);
void loadPointLight(FILE *inFP, entity_t *ent, char *token);
void completePointLight(scene_t *scene, entity_t *ent);
void dumpPointLight(FILE *outFP, entity_t *ent);
intensity_t processPointLight(scene_t *scene, entity_t *ent, entity_t *light, hitinfo_t *hit);

#define POINTLIGHT_T 1357864

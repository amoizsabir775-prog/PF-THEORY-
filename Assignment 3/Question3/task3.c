#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Property options:
// 0 = area, 1 = perimeter or length, 2 = distance from (0, 0)
struct dot {
    int type;
    float x;
    float y;
};

struct line {
    int type;
    float x1;
    float y1;
    float x2;
    float y2;
};

struct triangle {
    int type;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
};

struct square {
    int type;
    float x1;
    float y1;
    float side;
};

struct rectangle {
    int type;
    float x1;
    float y1;
    float width;
    float height;
};

float get_dist(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

float calculateProperty(void *shapePtr, int propertyType) {
    if (shapePtr == NULL) {
        return 0.0f;
    }

    // Har struct ka pehla member 'int type' hai, isliye pehle type check kar rahe hain
    int type = *(int *)shapePtr;

    switch (type) {
        case 0: { // Dot
            struct dot *d = (struct dot *)shapePtr;
            if (propertyType == 0) return 0.0f;
            if (propertyType == 1) return 0.0f;
            if (propertyType == 2) return get_dist(d->x, d->y, 0.0f, 0.0f);
            break;
        }

        case 1: { // Line
            struct line *l = (struct line *)shapePtr;
            if (propertyType == 0) return 0.0f;
            if (propertyType == 1) return get_dist(l->x1, l->y1, l->x2, l->y2);
            if (propertyType == 2) return get_dist(l->x1, l->y1, 0.0f, 0.0f);
            break;
        }

        case 2: { // Triangle
            struct triangle *t = (struct triangle *)shapePtr;
            if (propertyType == 0) {
                float area2 = t->x1 * (t->y2 - t->y3) +
                              t->x2 * (t->y3 - t->y1) +
                              t->x3 * (t->y1 - t->y2);
                if (area2 < 0) area2 = -area2;
                return 0.5f * area2;
            }
            if (propertyType == 1) {
                float a = get_dist(t->x1, t->y1, t->x2, t->y2);
                float b = get_dist(t->x2, t->y2, t->x3, t->y3);
                float c = get_dist(t->x3, t->y3, t->x1, t->y1);
                return a + b + c;
            }
            if (propertyType == 2) {
                return get_dist(t->x1, t->y1, 0.0f, 0.0f);
            }
            break;
        }

        case 3: { // Square
            struct square *s = (struct square *)shapePtr;
            if (propertyType == 0) return s->side * s->side;
            if (propertyType == 1) return 4.0f * s->side;
            if (propertyType == 2) return get_dist(s->x1, s->y1, 0.0f, 0.0f);
            break;
        }

        case 4: { // Rectangle
            struct rectangle *r = (struct rectangle *)shapePtr;
            if (propertyType == 0) return r->width * r->height;
            if (propertyType == 1) return 2.0f * (r->width + r->height);
            if (propertyType == 2) return get_dist(r->x1, r->y1, 0.0f, 0.0f);
            break;
        }
    }

    return 0.0f;
}

int main() {
    void *shapes[5];

    struct dot *d = malloc(sizeof(struct dot));
    d->type = 0;
    d->x = 6.f;
    d->y = 8.0f;
    shapes[0] = d;

    struct line *l = malloc(sizeof(struct line));
    l->type = 1;
    l->x1 = 0.1f; l->y1 = 0.2f;
    l->x2 = 3.0f; l->y2 = 3.0f;
    shapes[1] = l;

    struct triangle *t = malloc(sizeof(struct triangle));
    t->type = 2;
    t->x1 = 0.0f; t->y1 = 0.0f;
    t->x2 = 4.0f; t->y2 = 0.0f;
    t->x3 = 0.0f; t->y3 = 3.0f;
    shapes[2] = t;

    struct square *s = malloc(sizeof(struct square));
    s->type = 3;
    s->x1 = 1.4f;
    s->y1 = 4.0f;
    s->side = 2.0f;
    shapes[3] = s;

    struct rectangle *r = malloc(sizeof(struct rectangle));
    r->type = 4;
    r->x1 = 2.0f;
    r->y1 = 2.0f;
    r->width = 6.0f;
    r->height = 4.0f;
    shapes[4] = r;

    char *names[5] = {"Dot", "Line", "Triangle", "Square", "Rectangle"};

    printf("Shape       Area       Perimeter  OriginDist\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        float area = calculateProperty(shapes[i], 0);
        float perim = calculateProperty(shapes[i], 1);
        float dist = calculateProperty(shapes[i], 2);
        printf("%-11s %-10.2f %-10.2f %-10.2f\n", names[i], area, perim, dist);
    }

    for (int i = 0; i < 5; i++) {
        free(shapes[i]);
    }

    return 0;
}
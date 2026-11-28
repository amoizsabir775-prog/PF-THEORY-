#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int type;      
    double x;
    double y;
} Dot;

typedef struct {
    int type;     
    double x1, y1;
    double x2, y2;
} Line;


double cross_product(Dot o, Dot a, Dot b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

/* Find outer boundary Dot vertices in counter-clockwise order (Gift Wrapping) */
int compute_convex_hull(const Dot *pts, int n, Dot *hull) {
    /* Start at the leftmost point (smallest x; tie-break by smallest y) */
    int start = 0;
    for (int i = 1; i < n; i++) {
        if (pts[i].x < pts[start].x ||
            (pts[i].x == pts[start].x && pts[i].y < pts[start].y)) {
            start = i;
        }
    }

    int p = start;
    int hull_count = 0;

    do {
        hull[hull_count] = pts[p];
        hull[hull_count].type = 0;
        hull_count++;

        int q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
            double cp = cross_product(pts[p], pts[i], pts[q]);
            if (cp > 0.0) {
                /* pts[i] is more counter-clockwise than pts[q] */
                q = i;
            }
        }

        p = q;
    } while (p != start && hull_count <= n);

    return hull_count;
}

void build_hull_lines(const Dot *hull, int hull_count, Line *edges) {
    for (int i = 0; i < hull_count; i++) {
        int next = (i + 1) % hull_count;
        edges[i].type = 1;
        edges[i].x1 = hull[i].x;
        edges[i].y1 = hull[i].y;
        edges[i].x2 = hull[next].x;
        edges[i].y2 = hull[next].y;
    }
}

int main(void) {
    FILE *fp = fopen("points.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open points.txt\n");
        return 1;
    }

    int n = 0;
    if (fscanf(fp, "%d", &n) != 1 || n < 3) {
        printf("Error: First line must have N >= 3.\n");
        fclose(fp);
        return 1;
    }

    
    Dot *pts = (Dot *)malloc((size_t)n * sizeof(Dot));
    if (pts == NULL) {
        printf("Error: Memory allocation failed for points.\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%lf,%lf", &pts[i].x, &pts[i].y) != 2) {
            printf("Error: Invalid coordinate format at point %d.\n", i + 1);
            free(pts);
            fclose(fp);
            return 1;
        }
        pts[i].type = 0;
    }
    fclose(fp);

    
    Dot *hull = (Dot *)calloc((size_t)n, sizeof(Dot));
    Line *edges = (Line *)calloc((size_t)n, sizeof(Line));
    if (hull == NULL || edges == NULL) {
        printf("Error: Memory allocation failed for hull/edges.\n");
        free(hull);
        free(edges);
        free(pts);
        return 1;
    }

    int hull_count = compute_convex_hull(pts, n, hull);
    build_hull_lines(hull, hull_count, edges);

    printf("Convex Hull Vertices (Dot, type = 0):\n");
    for (int i = 0; i < hull_count; i++) {
        printf("  %d. (%.4f, %.4f) [type=%d]\n",
               i + 1, hull[i].x, hull[i].y, hull[i].type);
    }

    printf("\nConvex Hull Edges (Line, type = 1):\n");
    for (int i = 0; i < hull_count; i++) {
        printf("  %d. (%.4f, %.4f) -> (%.4f, %.4f) [type=%d]\n",
               i + 1, edges[i].x1, edges[i].y1, edges[i].x2, edges[i].y2, edges[i].type);
    }


    free(edges);
    free(hull);
    free(pts);

    return 0;
}
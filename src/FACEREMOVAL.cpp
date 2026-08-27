// Face-clipping and embedded-area helpers used by generated model geometry.

#define FACEREMOVAL_OWNER_DECLARATIONS
#include "FACEREMOVAL.h"
#undef FACEREMOVAL_OWNER_DECLARATIONS
#include "MEM.h"


static FP *GetIntersection(FP *first1, FP *first2);
static int CrossProduct(Vector &a0, Vector &a1, Vector &b0, Vector &b1);

// === Function implementations (address order) ===

/**
 * FaceRemoval_Setup__Fv
 *
 * Address:   0x80051F0C
 * Mangled:   FaceRemoval_Setup__Fv
 * Demangled: FaceRemoval_Setup(void)
 * Allocate the fixed face-point pool and reset its cursor.
 *
 */
void FaceRemoval_Setup(void) {
    FPBuf = static_cast<FP *>(
        Mem_New(sizeof(FP) * FaceRemovalPoolCapacity, 1, 1));
    CurrentFP = FPBuf;
}


/**
 * FaceRemoval_Cleanup__Fv
 *
 * Address:   0x80051F38
 * Mangled:   FaceRemoval_Cleanup__Fv
 * Demangled: FaceRemoval_Cleanup(void)
 * Release the face-point pool and clear its base pointer.
 */
void FaceRemoval_Cleanup(void) {
    Mem_Delete(FPBuf);
    FPBuf = 0;
}


/**
 * NewFP__Fv
 *
 * Address:   0x80051F5C
 * Mangled:   NewFP__Fv
 * Demangled: NewFP(void)
 * Allocate and initialize the next point in the fixed pool.
 *
 */
FP *NewFP(void) {
    // The target reuses one scalar register for the active flag and next cursor.
    union {
        int active;
        FP *next;
    } cursor;
    cursor.active = 1;
    NumUsedFps = NumUsedFps + 1;
    CurrentFP->in = cursor.active;
    FP *point = CurrentFP;
    cursor.next = point + 1;
    point->next = 0;
    point->next2 = 0;
    CurrentFP = cursor.next;
    return point;
}


/**
 * ResetFPList__Fv
 *
 * Address:   0x80051F94
 * Mangled:   ResetFPList__Fv
 * Demangled: ResetFPList(void)
 * Reset the face-point pool cursor and allocation count.
 */
void ResetFPList(void) {
    NumUsedFps = 0;
    CurrentFP = FPBuf;
}

/** Returns whether two face-removal points occupy the same rounded position. */
static inline int PointsAreShared(FP *point1, FP *point2) {
    int diffx;
    int diffy;
    int diffz;

    diffx = point1->pt.vx - point2->pt.vx;
    if ((unsigned int)(diffx + 3) >= 7) {
        return 0;
    }

    diffy = point1->pt.vy - point2->pt.vy;
    if ((unsigned int)(diffy + 3) >= 7) {
        return 0;
    }

    diffz = point1->pt.vz - point2->pt.vz;
    return (unsigned int)(diffz + 3) < 7;
}

/** Returns the signed two-dimensional cross product between two point pairs. */
static inline int FaceCrossProduct(Vector &a0, Vector &a1, Vector &b0, Vector &b1) {
    return (a1.vx - a0.vx) * (b1.vy - b0.vy) -
           (a1.vy - a0.vy) * (b1.vx - b0.vx);
}

/** Returns the two-dimensional dot product between two point-pair directions. */
static inline int FaceDotProduct(Vector &a0, Vector &a1, Vector &b0, Vector &b1) {
    int dot = (a1.vx - a0.vx) * (b1.vx - b0.vx) +
              (a1.vy - a0.vy) * (b1.vy - b0.vy);
    return dot;
}


/**
 * GetEmbeddedArea__FP2FPT0
 *
 * Address:   0x80051FA8
 * Mangled:   GetEmbeddedArea__FP2FPT0
 * Demangled: GetEmbeddedArea(FP *, FP *)
 *
 * @param points1  First circular face-point list.
 * @param points2  Second circular face-point list.
 * @return Area of the embedded polygon, or zero when no closed area exists.
 */
int GetEmbeddedArea(FP *points1, FP *points2) {
    FP *points[2];
    FP *start;
    int other_face;
    FP *other_first;
    FP *t;

    points[0] = points1;
    points[1] = points2;

    if (points1->pt.vz - points2->pt.vz >= 2 ||
        points2->pt.vz - points1->pt.vz >= 2) {
        return 0;
    }

    start = 0;
    {
        int face;
        int other_index;
        FP **face_cursor;
        FP **other_cursor;

        face = 0;
        other_index = 1;
        face_cursor = points;
        other_cursor = points + 1;

        for (; face < 2; face++) {
            FP *A = *face_cursor;
            FP *B = *other_cursor;
            FP *t1 = A;

            do {
                FP *t2 = B;

                do {
                    FP *list;

                    if (CrossProduct(t2->pt, t2->next->pt, t2->next->pt, t1->pt) < 0) {
                        int inside = 1;
                        if (t1->in == inside) {
                            t1->in = 0;
                        }
                    }

                    list = GetIntersection(t1, t2);
                    if (list) {
                        FP *temp = list;

                        temp->in = 2;
                        while (temp->next) {
                            temp->in = 2;
                            temp = temp->next;
                        }

                        temp->next = t1->next;
                        t1->next = temp;
                    }

                    t2 = t2->next;
                } while (t2 != B);

                if (t1->in && !start) {
                    start = t1;
                    other_face = other_index;
                }

                t1 = t1->next;
            } while (t1 != A);

            other_cursor--;
            other_index--;
            face_cursor++;
        }
    }

    if (!start) {
        return 0;
    }

    other_first = points[other_face];
    t = start;
    do {
        FP *other = other_first;
        int shared = 0;

        do {
            if (PointsAreShared(t, other)) {
                shared = 1;
                goto shared_point_found;
            }
            other = other->next;
        } while (other != other_first);

shared_point_found:
        if (shared) {
            t->in = 1;
            other->in = 1;
            t->next2 = other->next;
            other->next2 = t->next;
        }

        t = t->next;
    } while (t != start);

    t = start;
    for (;;) {
        FP *other = t->next2;

        if (other) {
            if (!t->next->in) {
                if (!other->in) {
                    return 0;
                }
                t->next = other;
            } else if (other->in) {
                t->next = other;
            }
        }

        if (PointsAreShared(t->next, start)) {
            t->next = start;
            return GetArea(start);
        }

        t = t->next;
    }
}


/**
 * GetArea__FP2FP
 *
 * Address:   0x800522C8
 * Mangled:   GetArea__FP2FP
 * Demangled: GetArea(FP *)
 *
 * @param list  Circular polygon point list.
 * @return Absolute area of the polygon.
 */
int GetArea(FP *list) {
    int area = 0;
    FP *point = list;
    do {
        area += point->pt.vx * point->next->pt.vy -
                point->next->pt.vx * point->pt.vy;
        point = point->next;
    } while (point != list);
    area >>= 1;
    if (area < 0) return -area;
    return area;
}


/**
 * GetIntersection__FP2FPT0
 *
 * Address:   0x8005232C
 * Mangled:   GetIntersection__FP2FPT0
 * Demangled: GetIntersection(FP *, FP *)
 *            Computes intersection of two line segments from polygon edge lists
 *
 * @param first1  First directed polygon edge.
 * @param first2  Second directed polygon edge.
 * @return Newly allocated intersection point list, or null when the edges do not intersect.
 */
static FP *GetIntersection(FP *first1, FP *first2) {
    FP *add1;
    FP *add2;
    Vector *b = &first1->next->pt;
    Vector *c = &first2->pt;
    Vector *d = &first2->next->pt;
    int cross1 = FaceCrossProduct(first1->pt, *b, *b, *c);
    int cross2 = FaceCrossProduct(first1->pt, *b, *b, *d);
    int cross3 = FaceCrossProduct(*c, *d, *d, first1->pt);
    int cross4 = FaceCrossProduct(*c, *d, *d, *b);
    char test1;
    char test2;
    int r_num;
    int r_dnom;

    add1 = 0;
    add2 = 0;

    if (!cross1 && !cross2 && !cross3 && !cross4) {
        int abc = FaceDotProduct(first1->pt, *b, *b, *c) >= 0;
        int abd = FaceDotProduct(first1->pt, *b, *b, *d) >= 0;
        int bac = FaceDotProduct(*b, first1->pt, first1->pt, *c) >= 0;
        int bad = FaceDotProduct(*b, first1->pt, first1->pt, *d) >= 0;
        int cda = FaceDotProduct(*c, *d, *d, first1->pt) >= 0;
        int cdb = FaceDotProduct(*c, *d, *d, *b) >= 0;
        int dca = FaceDotProduct(*d, *c, *c, first1->pt) >= 0;
        int dcb = FaceDotProduct(*d, *c, *c, *b) >= 0;

        if (abc == abd && bac == bad && cda != cdb && dca != dcb) {
            add1 = NewFP();
            add1->pt = first2->pt;
            add2 = NewFP();
            add2->pt = *d;
        } else if (abc != abd && bac == bad && cda == cdb && dca != dcb) {
            add1 = NewFP();
            add1->pt = *c;
        } else if (abc == abd && bac != bad && cda != cdb && dca == dcb) {
            add1 = NewFP();
            add1->pt = *d;
        }

        if (PointsAreShared(add1, first1) || PointsAreShared(add1, first1->next)) {
            add1 = 0;
        }
        if (PointsAreShared(add2, first1) || PointsAreShared(add2, first1->next)) {
            add2 = 0;
        }

        if (add1) {
            if (add2) {
                add1->next = add2;
            }
        } else if (add2) {
            add1 = add2;
        }
        return add1;
    }

    test1 = 0;
    if ((cross1 <= 0 && cross2 >= 0) || (cross1 >= 0 && cross2 <= 0)) {
        test1 = 1;
    }

    test2 = 0;
    if ((cross3 <= 0 && cross4 >= 0) || (cross3 >= 0 && cross4 <= 0)) {
        test2 = 1;
    }

    if (PointsAreShared(first1, first2) ||
        PointsAreShared(first1, first2->next) ||
        PointsAreShared(first1->next, first2) ||
        PointsAreShared(first1->next, first2->next)) {
        return 0;
    }

    if (!test1 || !test2) {
        return 0;
    }

    add1 = NewFP();
    r_num = (c->vy - first1->pt.vy) * (b->vx - first1->pt.vx) -
            (c->vx - first1->pt.vx) * (b->vy - first1->pt.vy);
    r_dnom = (d->vx - c->vx) * (b->vy - first1->pt.vy) -
             (d->vy - c->vy) * (b->vx - first1->pt.vx);
    r_num <<= 6;
    r_dnom >>= 4;
    r_num /= r_dnom;

    add1->pt.vx = c->vx + ((r_num * (d->vx - c->vx)) >> 10);
    add1->pt.vy = c->vy + ((r_num * (d->vy - c->vy)) >> 10);
    add1->pt.vz = c->vz;

    if (PointsAreShared(add1, first1) || PointsAreShared(add1, first1->next)) {
        add1 = 0;
    }
    return add1;
}


/**
 * CrossProduct__FR6VectorN30
 *
 * Address:   0x80052D28
 * Mangled:   CrossProduct__FR6VectorN30
 * Demangled: CrossProduct(Vector &, Vector &, Vector &, Vector &)
 *            Returns (b.x-a.x)*(d.y-c.y) - (b.y-a.y)*(d.x-c.x)
 *
 * @param a0  First point of the first directed pair.
 * @param a1  Second point of the first directed pair.
 * @param b0  First point of the second directed pair.
 * @param b1  Second point of the second directed pair.
 * @return Signed two-dimensional cross product of the pair directions.
 */
static int CrossProduct(Vector &a0, Vector &a1, Vector &b0, Vector &b1) {
    return FaceCrossProduct(a0, a1, b0, b1);
}

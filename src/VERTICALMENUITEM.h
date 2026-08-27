#ifndef VERTICALMENUITEM_INC_H
#define VERTICALMENUITEM_INC_H

struct Sprite;

/** Text metrics, position, and erase backing for one vertical-menu entry. */
struct VerticalMenuItem {
    struct Sprite *erase_sprite;
    char string[32];
    int x;
    int y;
    int w;
    int h;
    int baseline;
    int grayed;
    int erase_sprite_x;
};

typedef char VerticalMenuItem_size_must_be_64[
    (sizeof(struct VerticalMenuItem) == 64) ? 1 : -1];
typedef char VerticalMenuItem_erase_sprite_offset_must_be_0[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->erase_sprite) == 0)
        ? 1
        : -1];
typedef char VerticalMenuItem_string_offset_must_be_4[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->string) == 4) ? 1 : -1];
typedef char VerticalMenuItem_x_offset_must_be_36[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->x) == 36) ? 1 : -1];
typedef char VerticalMenuItem_y_offset_must_be_40[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->y) == 40) ? 1 : -1];
typedef char VerticalMenuItem_w_offset_must_be_44[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->w) == 44) ? 1 : -1];
typedef char VerticalMenuItem_h_offset_must_be_48[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->h) == 48) ? 1 : -1];
typedef char VerticalMenuItem_baseline_offset_must_be_52[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->baseline) == 52) ? 1 : -1];
typedef char VerticalMenuItem_grayed_offset_must_be_56[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->grayed) == 56) ? 1 : -1];
typedef char VerticalMenuItem_erase_sprite_x_offset_must_be_60[
    ((unsigned long)&(((struct VerticalMenuItem *)0)->erase_sprite_x) == 60)
        ? 1
        : -1];

#endif // VERTICALMENUITEM_INC_H

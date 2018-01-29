#ifndef INC_ICON_H
#define INC_ICON_H

extern int32 image_qbicon16_handle;
extern int32 image_qbicon32_handle;

extern int32 image_qbicon16_w;
extern int32 image_qbicon16_h;
extern int32 image_qbicon32_w;
extern int32 image_qbicon32_h;

void sub__icon(int32 handle_icon, int32 handle_window_icon, int32 passed);
void init_icons();

#endif

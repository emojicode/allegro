#include <emojicode/runtime/Runtime.h>
#include <emojicode/s/String.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

class App : public runtime::Object<App> {

};

template <typename T, int Unique = 0>
class AllegroWrapper : public runtime::Object<AllegroWrapper<T, Unique>> {
public:
    AllegroWrapper(T value) : value(value) {}
    T value;
};

using Bitmap = AllegroWrapper<ALLEGRO_BITMAP*>;
using Display = AllegroWrapper<ALLEGRO_DISPLAY*>;
using Font = AllegroWrapper<ALLEGRO_FONT*>;
using Queue = AllegroWrapper<ALLEGRO_EVENT_QUEUE*>;
using EventCharacter = AllegroWrapper<ALLEGRO_EVENT>;
using EventKeyDown = AllegroWrapper<ALLEGRO_EVENT, 1>;
using EventKeyUp = AllegroWrapper<ALLEGRO_EVENT, 2>;
using MouseMove = AllegroWrapper<ALLEGRO_EVENT, 3>;
using MouseDown = AllegroWrapper<ALLEGRO_EVENT, 4>;
using MouseUp = AllegroWrapper<ALLEGRO_EVENT, 5>;

struct Color {
    runtime::Real r;
    runtime::Real g;
    runtime::Real b;
    runtime::Real a;

    ALLEGRO_COLOR allegroColor() const {
        return al_map_rgba_f(r, g, b, a);
    }
};

runtime::Callable<void, App*> mainCallable;

int allegroMain(int, char**) {
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    mainCallable(App::init());
    return 0;
}

extern "C" void allegroSetup(runtime::ClassInfo *, decltype(mainCallable) callable) {
    mainCallable = std::move(callable);
    al_run_main(0, nullptr, allegroMain);
}

extern "C" void allegroAppFlip(App *app) {
    al_flip_display();
}

extern "C" void allegroAppSetTargetBitmap(App *app, Bitmap *bitmap) {
    al_set_target_bitmap(bitmap->value);
}

extern "C" void allegroAppSetTargetBackbuffer(App *app, Display *display) {
    al_set_target_backbuffer(display->value);
}

extern "C" void allegroAppClear(App *app, const Color *color) {
    al_clear_to_color(color->allegroColor());
}

extern "C" void allegroAppDrawLine(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2, runtime::Real y2,
                                   runtime::Real thickness, const Color *color) {
    al_draw_line(x1, y1, x2, y2, color->allegroColor(), thickness);
}

extern "C" void allegroAppDrawTriangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2, runtime::Real y2,
                                       runtime::Real x3, runtime::Real y3, runtime::Real thickness, const Color *color) {
    al_draw_triangle(x1, y1, x2, y2, x3, y3, color->allegroColor(), thickness);
}

extern "C" void allegroAppDrawFilledTriangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2,
                                             runtime::Real y2, runtime::Real x3, runtime::Real y3, const Color *color) {
    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, color->allegroColor());
}

extern "C" void allegroAppDrawRectangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2, runtime::Real y2,
                                   runtime::Real thickness, const Color *color) {
    al_draw_rectangle(x1, y1, x2, y2, color->allegroColor(), thickness);
}

extern "C" void allegroAppDrawFilledRectangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2,
    runtime::Real y2, const Color *color) {
    al_draw_filled_rectangle(x1, y1, x2, y2, color->allegroColor());
}

extern "C" void allegroAppDrawCircle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real r,
                                   runtime::Real thickness, const Color *color) {
    al_draw_circle(x1, y1, r, color->allegroColor(), thickness);
}

extern "C" void allegroAppDrawFilledCircle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real r,
                                           const Color *color) {
    al_draw_filled_circle(x1, y1, r, color->allegroColor());
}

extern "C" void allegroAppDrawRoundedRectangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2, runtime::Real y2,
                                   runtime::Real rx, runtime::Real ry, runtime::Real thickness, const Color *color) {
    al_draw_rounded_rectangle(x1, y1, x2, y2, rx, ry, color->allegroColor(), thickness);
}

extern "C" void allegroAppDrawFilledRoundedRectangle(App *app, runtime::Real x1, runtime::Real y1, runtime::Real x2, runtime::Real y2,
                                   runtime::Real rx, runtime::Real ry, const Color *color) {
    al_draw_filled_rounded_rectangle(x1, y1, x2, y2, rx, ry, color->allegroColor());
}

extern "C" void allegroAppDrawBitmap(App *app, Bitmap *bitmap, runtime::Real dx, runtime::Real dy) {
    al_draw_bitmap(bitmap->value, dx, dy, 0);
}

extern "C" void allegroAppDrawScaledBitmap(App *app, Bitmap *bitmap, runtime::Real sx, runtime::Real sy, runtime::Real sw, runtime::Real sh, runtime::Real dx, runtime::Real dy, runtime::Real dw, runtime::Real dh) {
    al_draw_scaled_bitmap(bitmap->value, sx, sy, sw, sh, dx, dy, dw, dh, 0);
}

extern "C" void allegroAppDrawText(App *app, Font *font, s::String *text, runtime::Real x, runtime::Real y,
                                   runtime::Enum align, const Color *color) {
    al_draw_text(font->value, color->allegroColor(), x, y, align, text->stdString().c_str());
}

extern "C" runtime::SimpleError<Bitmap*> allegroNewBitmapPath(s::String *string) {
    return Bitmap::init(al_load_bitmap(string->stdString().c_str()));
}

extern "C" runtime::SimpleError<Bitmap*> allegroNewBitmapSize(runtime::Integer w, runtime::Integer h) {
    return Bitmap::init(al_create_bitmap(w, h));
}

extern "C" Display* allegroNewDisplay(runtime::Integer width, runtime::Integer height) {
    return Display::init(al_create_display(width, height));
}

extern "C" void allegroDisplaySetTitle(Display *display, s::String *title) {
    al_set_window_title(display->value, title->stdString().c_str());
}

extern "C" runtime::SimpleError<Font*> allegroNewFont(s::String *path, runtime::Integer size) {
    return Font::init(al_load_font(path->stdString().c_str(), size, 0));
}

extern "C" Queue* allegroNewEventQueue() {
    return Queue::init(al_create_event_queue());
}

extern "C" void* allegroEventQueueWait(Queue *queue) {
    ALLEGRO_EVENT event;
    while (true) {
        al_wait_for_event(queue->value, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_KEY_CHAR:
                return EventCharacter::init(std::move(event));
            case ALLEGRO_EVENT_KEY_DOWN:
                return EventKeyDown::init(std::move(event));
            case ALLEGRO_EVENT_KEY_UP:
                return EventKeyUp::init(std::move(event));
            case ALLEGRO_EVENT_MOUSE_AXES:
                return MouseMove::init(std::move(event));
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                return MouseDown::init(std::move(event));
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                return MouseUp::init(std::move(event));
            // case ALLEGRO_EVENT_TIMER:
            //     event->klass = CL_EVENT_TIMER;
            //     break;
            default:
                break;
        }
    }
}

extern "C" void allegroEventQueueRegisterMouse(Queue *queue) {
    al_register_event_source(queue->value, al_get_mouse_event_source());
}

extern "C" void allegroEventQueueRegisterKeyboard(Queue *queue) {
    al_register_event_source(queue->value, al_get_keyboard_event_source());
}

extern "C" void allegroEventQueueRegisterTimer() {

}

extern "C" runtime::Integer allegroKeyPressCode(EventCharacter *event) {
    return event->value.keyboard.keycode;
}

extern "C" runtime::Integer allegroKeyUpCode(EventCharacter *event) {
    return event->value.keyboard.keycode;
}

extern "C" runtime::Integer allegroKeyDownCode(EventCharacter *event) {
    return event->value.keyboard.keycode;
}

extern "C" runtime::SimpleOptional<s::String*> allegroKeyPressString(EventCharacter *event) {
    auto c = event->value.keyboard.unichar;
    if (c < 1) {
        return runtime::NoValue;
    }
    auto newString = s::String::init();
    newString->count = 1;
    newString->characters = runtime::allocate<char>(1);
    newString->characters.get()[0] = c;
    return newString;
}

extern "C" runtime::Boolean allegroKeyPressRepeated(EventCharacter *event) {
    return event->value.keyboard.repeat;
}

extern "C" runtime::Real allegroMouseEventX(MouseMove *event) {
    return event->value.mouse.x;
}

extern "C" runtime::Real allegroMouseEventY(MouseMove *event) {
    return event->value.mouse.y;
}

SET_INFO_FOR(App, allegro, 1f3d4)
SET_INFO_FOR(Bitmap, allegro, 1f5bc)
SET_INFO_FOR(Display, allegro, 1f4fa)
SET_INFO_FOR(Font, allegro, 1f549)
SET_INFO_FOR(Queue, allegro, 1f5c3)
SET_INFO_FOR(EventCharacter, allegro, 1f4e9)
SET_INFO_FOR(EventKeyUp, allegro, 1f4e4)
SET_INFO_FOR(EventKeyDown, allegro, 1f4e5)
SET_INFO_FOR(MouseMove, allegro, 2747)
SET_INFO_FOR(MouseUp, allegro, 1f51b)
SET_INFO_FOR(MouseDown, allegro, 1f51d)

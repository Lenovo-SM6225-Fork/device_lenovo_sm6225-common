#include <gui/SurfaceComposerClient.h>
#include <ui/Size.h>

extern "C" {

android::ui::Size _ZN7android2ui4Size7INVALIDE;

void
_ZN7android21SurfaceComposerClient11Transaction20setDisplayLayerStackERKNS_2spINS_7IBinderEEENS_2ui10LayerStackE(
    const android::sp<android::IBinder>& token,
    android::ui::LayerStack layerStack);

void
_ZN7android21SurfaceComposerClient11Transaction20setDisplayLayerStackERKNS_2spINS_7IBinderEEEj(
    const android::sp<android::IBinder>& token, uint32_t layerStack) {
  _ZN7android21SurfaceComposerClient11Transaction20setDisplayLayerStackERKNS_2spINS_7IBinderEEENS_2ui10LayerStackE(
      token, android::ui::LayerStack::fromValue(layerStack));
}
}

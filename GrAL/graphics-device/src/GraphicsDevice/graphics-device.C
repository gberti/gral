
// $LICENSE

#include "GraphicsDevice/graphics-device.h"
#include "GraphicsDevice/translator.h"

// class for renderers expecting data from a stream,
// e.g. a file or a pipe (like geomview)

class renderer_file_device : public graphics_device_impl {
public:
  renderer_file_device(rendering_language* r,FileDevice Dev)
    : renderer(r), dev(Dev) {}
private:
  rendering_language* renderer;
  FileDevice          dev;        
};

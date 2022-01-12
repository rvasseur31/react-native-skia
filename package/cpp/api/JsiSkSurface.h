#pragma once

#include "JsiSkHostObjects.h"
#include <jsi/jsi.h>

#include <JsiSkSurfaceFactory.h>
#include <JsiSkImage.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <SkSurface.h>

#pragma clang diagnostic pop

namespace RNSkia {

using namespace facebook;

class JsiSkSurface : public JsiSkWrappingSkPtrHostObject<SkSurface> {
public:
    JsiSkSurface(std::shared_ptr<RNSkPlatformContext> context,
                 sk_sp<SkSurface> surface)
            : JsiSkWrappingSkPtrHostObject<SkSurface>(context, surface) {}

    // TODO: declare in JsiSkWrappingSkPtrHostObject via extra template parameter?
    JSI_PROPERTY_GET(__typename__) {
            return jsi::String::createFromUtf8(runtime, "Surface");
    }

    JSI_HOST_FUNCTION(getCanvas) {
        return jsi::Object::createFromHostObject(runtime,
        std::make_shared<JsiSkCanvas>(getContext(),
                                      getObject()->getCanvas()));
    }

    JSI_HOST_FUNCTION(makeImageSnapshot) {
        auto image = getObject()->makeImageSnapshot();
        return jsi::Object::createFromHostObject(runtime, std::make_shared<JsiSkImage>(getContext(), image));
    }

    JSI_EXPORT_PROPERTY_GETTERS(JSI_EXPORT_PROP_GET(JsiSkSurface, __typename__))
    JSI_EXPORT_FUNCTIONS(JSI_EXPORT_FUNC(JsiSkSurface, getCanvas),
                         JSI_EXPORT_FUNC(JsiSkSurface, makeImageSnapshot))

    /**
      Returns the underlying object from a host object of this type
     */
    static sk_sp<SkSurface> fromValue(jsi::Runtime &runtime, const jsi::Value &obj) {
        return obj.asObject(runtime)
                .asHostObject<JsiSkSurface>(runtime)
                .get()
                ->getObject();
    }
};

} // namespace RNSkia
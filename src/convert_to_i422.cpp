#include "convert_to_i422.h"
#include "convert_manager.h"

int32_t ConvertToI422::register_converter()
{

    Converter i420_yv12_to_i422 = [](
                                      SoftwareFrameWithMemory& source,
                                      SoftwareFrameWithMemory& dest,
                                      RotationMode rotate,
                                      const CropArea& crop_area) -> int32_t {
        int8_t u_plane_index = (source.format == kSoftwareFormatI420) ? 1 : 2;
        int8_t v_plane_index = (source.format == kSoftwareFormatI420) ? 2 : 1;
        libyuv::I420ToI422(source.data[0], source.line_size[0],
            source.data[u_plane_index], source.line_size[u_plane_index],
            source.data[v_plane_index], source.line_size[v_plane_index],
            dest.data[0], dest.line_size[0],
            dest.data[1], dest.line_size[1],
            dest.data[2], dest.line_size[2],
            source.width, source.height);
        return 0;
    };
    ConvertManager::add_converter(kSoftwareFormatI420, kSoftwareFormatI422, i420_yv12_to_i422);
    ConvertManager::add_converter(kSoftwareFormatYV12, kSoftwareFormatI422, i420_yv12_to_i422);

    Converter nv12_nv21_to_i422 = [](
                                      SoftwareFrameWithMemory& source,
                                      SoftwareFrameWithMemory& dest,
                                      RotationMode rotate,
                                      const CropArea& crop_area) -> int32_t {
        int8_t u_plane_index = (source.format == kSoftwareFormatNV12) ? 1 : 2;
        int8_t v_plane_index = (source.format == kSoftwareFormatNV12) ? 2 : 1;

        SoftwareFrameWithMemory i420 = { { kSoftwareFormatI420, source.width, source.height } };
        i420.alloc();
        libyuv::SplitUVPlane(source.data[1], source.line_size[1],
            i420.data[u_plane_index], i420.line_size[u_plane_index],
            i420.data[v_plane_index], i420.line_size[v_plane_index],
            source.width / 2, source.height / 2);

        libyuv::I420ToI422(source.data[0], source.line_size[0],
            i420.data[1], i420.line_size[1],
            i420.data[2], i420.line_size[2],
            dest.data[0], dest.line_size[0],
            dest.data[1], dest.line_size[1],
            dest.data[2], dest.line_size[2],
            source.width, source.height);
        return 0;
    };
    ConvertManager::add_converter(kSoftwareFormatNV12, kSoftwareFormatI422, nv12_nv21_to_i422);
    ConvertManager::add_converter(kSoftwareFormatNV21, kSoftwareFormatI422, nv12_nv21_to_i422);

    ConvertManager::add_converter(kSoftwareFormatI422, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::I422Copy(source.data[0], source.line_size[0],
                source.data[1], source.line_size[1],
                source.data[2], source.line_size[2],
                dest.data[0], dest.line_size[0],
                dest.data[1], dest.line_size[1],
                dest.data[2], dest.line_size[2],
                source.width, source.height);
            return 0;
        });

    Converter nv16_nv61_to_i422 = [](
                                      SoftwareFrameWithMemory& source,
                                      SoftwareFrameWithMemory& dest,
                                      RotationMode rotate,
                                      const CropArea& crop_area) -> int32_t {
        int8_t u_plane_index = (source.format == kSoftwareFormatNV16) ? 1 : 2;
        int8_t v_plane_index = (source.format == kSoftwareFormatNV16) ? 2 : 1;

        libyuv::SplitUVPlane(source.data[1], source.line_size[1],
            dest.data[u_plane_index], dest.line_size[u_plane_index],
            dest.data[v_plane_index], dest.line_size[v_plane_index],
            source.width / 2, source.height);
        libyuv::CopyPlane(source.data[0], source.line_size[0],
            dest.data[0], dest.line_size[0],
            source.width, source.height);
        return 0;
    };
    ConvertManager::add_converter(kSoftwareFormatNV16, kSoftwareFormatI422, nv16_nv61_to_i422);
    ConvertManager::add_converter(kSoftwareFormatNV61, kSoftwareFormatI422, nv16_nv61_to_i422);

    ConvertManager::add_converter(kSoftwareFormatYUYV422, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::YUY2ToI422(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                dest.data[1], dest.line_size[1],
                dest.data[2], dest.line_size[2],
                source.width, source.height);
            return 0;
        });

    ConvertManager::add_converter(kSoftwareFormatYVYU422, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            SoftwareFrameWithMemory yvyu = { { kSoftwareFormatYVYU422, source.width, source.height } };
            yvyu.alloc();
            libyuv::ARGBToABGR(source.data[0], source.line_size[0],
                yvyu.data[0], yvyu.line_size[0],
                source.width / 2, source.height);
            libyuv::YUY2ToI422(yvyu.data[0], yvyu.line_size[0],
                dest.data[0], dest.line_size[0],
                dest.data[1], dest.line_size[1],
                dest.data[2], dest.line_size[2],
                yvyu.width, yvyu.height);
            return 0;
        });
    ConvertManager::add_converter(kSoftwareFormatUYVY422, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::UYVYToI422(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                dest.data[1], dest.line_size[1],
                dest.data[2], dest.line_size[2],
                source.width, source.height);
            return 0;
        });

    ConvertManager::add_converter(kSoftwareFormatI444, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::CopyPlane(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                source.width, source.height);
            // half uv width of 444
            libyuv::ScalePlane(source.data[1], source.line_size[1], source.width, source.height,
                dest.data[1], dest.line_size[1], dest.width / 2, dest.height, libyuv::kFilterLinear);
            libyuv::ScalePlane(source.data[2], source.line_size[2], source.width, source.height,
                dest.data[2], dest.line_size[2], dest.width / 2, dest.height, libyuv::kFilterLinear);
            return 0;
        });

    Converter nv24_nv42_to_i422 = [](
                                      SoftwareFrameWithMemory& source,
                                      SoftwareFrameWithMemory& dest,
                                      RotationMode rotate,
                                      const CropArea& crop_area) -> int32_t {
        int8_t u_plane_index = (source.format == kSoftwareFormatNV24) ? 1 : 2;
        int8_t v_plane_index = (source.format == kSoftwareFormatNV24) ? 2 : 1;

        SoftwareFrameWithMemory i444 = { { kSoftwareFormatI444, source.width, source.height } };
        i444.alloc();
        libyuv::SplitUVPlane(source.data[1], source.line_size[1],
            i444.data[u_plane_index], i444.line_size[u_plane_index],
            i444.data[v_plane_index], i444.line_size[v_plane_index],
            source.width / 2, source.height);
        libyuv::CopyPlane(source.data[0], source.line_size[0],
            dest.data[0], dest.line_size[0],
            i444.width, i444.height);
        // half uv width of 444
        libyuv::ScalePlane(i444.data[1], i444.line_size[1], i444.width, i444.height,
            dest.data[1], dest.line_size[1], dest.width / 2, dest.height, libyuv::kFilterLinear);
        libyuv::ScalePlane(i444.data[2], i444.line_size[2], i444.width, i444.height,
            dest.data[2], dest.line_size[2], dest.width / 2, dest.height, libyuv::kFilterLinear);

        return 0;
    };
    ConvertManager::add_converter(kSoftwareFormatNV24, kSoftwareFormatI422, nv24_nv42_to_i422);
    ConvertManager::add_converter(kSoftwareFormatNV42, kSoftwareFormatI422, nv24_nv42_to_i422);

    ConvertManager::add_converter(kSoftwareFormatYUV444, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            SoftwareFrameWithMemory i444 = { { kSoftwareFormatI444, source.width, source.height } };
            i444.alloc();
            libyuv::SplitRGBPlane(source.data[0], source.line_size[0],
                i444.data[0], i444.line_size[0],
                i444.data[1], i444.line_size[1],
                i444.data[2], i444.line_size[2],
                source.width, source.height);

            libyuv::CopyPlane(i444.data[0], i444.line_size[0],
                dest.data[0], dest.line_size[0],
                i444.width, i444.height);
            // half uv width of 444
            libyuv::ScalePlane(i444.data[1], i444.line_size[1], i444.width, i444.height,
                dest.data[1], dest.line_size[1], dest.width / 2, dest.height, libyuv::kFilterLinear);
            libyuv::ScalePlane(i444.data[2], i444.line_size[2], i444.width, i444.height,
                dest.data[2], dest.line_size[2], dest.width / 2, dest.height, libyuv::kFilterLinear);
            return 0;
        });
    // RGB 24 32 bits use argb intermediate format is best way current.
    ConvertManager::add_converter(kSoftwareFormatBGRA32, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::ARGBToI422(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                dest.data[1], dest.line_size[1],
                dest.data[2], dest.line_size[2],
                source.width, source.height);
            return 0;
        });

    Converter rgb24_a32_to_argb = [](
                                      SoftwareFrameWithMemory& source,
                                      SoftwareFrameWithMemory& dest,
                                      RotationMode rotate,
                                      const CropArea& crop_area) -> int32_t {
        auto source_fourcc = libyuv_fourcc_maps[source.format];
        auto dest_fourcc = libyuv_fourcc_maps[dest.format];
        uint32_t crop_width = (crop_area.crop_width == UINT32_MAX) ? source.width : crop_area.crop_width;
        uint32_t crop_height = (crop_area.crop_height == UINT32_MAX) ? source.height : crop_area.crop_height;

        SoftwareFrameWithMemory bgra = { { kSoftwareFormatBGRA32, crop_width, crop_height } };
        bgra.alloc();
        libyuv::ConvertToARGB(source.data[0], 0,
            bgra.data[0], bgra.line_size[0],
            crop_area.crop_x, crop_area.crop_y,
            source.line_size[0] / source_fourcc.stride_unit_0, source.height,
            crop_width, crop_height,
            libyuv::kRotate0,
            source_fourcc.fourcc);

        libyuv::ARGBToI422(bgra.data[0], bgra.line_size[0],
            dest.data[0], dest.line_size[0],
            dest.data[1], dest.line_size[1],
            dest.data[2], dest.line_size[2],
            crop_width, crop_height);
        return 0;
    };
    ConvertManager::add_converter(kSoftwareFormatRGB24, kSoftwareFormatI422, rgb24_a32_to_argb);
    ConvertManager::add_converter(kSoftwareFormatBGR24, kSoftwareFormatI422, rgb24_a32_to_argb);
    ConvertManager::add_converter(kSoftwareFormatABGR32, kSoftwareFormatI422, rgb24_a32_to_argb);
    ConvertManager::add_converter(kSoftwareFormatARGB32, kSoftwareFormatI422, rgb24_a32_to_argb);
    ConvertManager::add_converter(kSoftwareFormatRGBA32, kSoftwareFormatI422, rgb24_a32_to_argb);

    ConvertManager::add_converter(kSoftwareFormatGRAY8, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            libyuv::CopyPlane(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                source.width, source.height);

            libyuv::SetPlane(source.data[1], source.line_size[1], source.width / 2, source.height, 128);
            libyuv::SetPlane(source.data[2], source.line_size[2], source.width / 2, source.height, 128);
            return 0;
        });
    ConvertManager::add_converter(kSoftwareFormatGRAY8A, kSoftwareFormatI422,
        [](SoftwareFrameWithMemory& source,
            SoftwareFrameWithMemory& dest,
            RotationMode rotate,
            const CropArea& crop_area) -> int32_t {
            SoftwareFrameWithMemory alpha = { { kSoftwareFormatGRAY8, source.width, source.height } };
            alpha.alloc();

            libyuv::SplitUVPlane(source.data[0], source.line_size[0],
                dest.data[0], dest.line_size[0],
                alpha.data[0], alpha.line_size[0],
                source.width, source.height);

            libyuv::SetPlane(source.data[1], source.line_size[1], source.width / 2, source.height, 128);
            libyuv::SetPlane(source.data[2], source.line_size[2], source.width / 2, source.height, 128);
            return 0;
        });
    return 0;
}
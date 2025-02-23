
#include "ffmpeg_wrapper.h"

extern "C" {
#include <gif_lib.h>
#include <png.h>
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        return -1;
    }

    std::string in_media = argv[1];
    std::string out_gif = argv[2];

    FFmpegWrapper::convertVideo2Gif(in_media, out_gif, 2.0, 4.3);

    return 0;
}

// 读取 PNG 图片并转换为 RGB 数据
// unsigned char *read_png_file(const char *filename, int *width, int *height)
// {
//     FILE *fp = fopen(filename, "rb");
//     if (!fp) {
//         fprintf(stderr, "Could not open PNG file: %s\n", filename);
//         return NULL;
//     }

//     png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//     if (!png) {
//         fclose(fp);
//         return NULL;
//     }

//     png_infop info = png_create_info_struct(png);
//     if (!info) {
//         png_destroy_read_struct(&png, NULL, NULL);
//         fclose(fp);
//         return NULL;
//     }

//     if (setjmp(png_jmpbuf(png))) {
//         png_destroy_read_struct(&png, &info, NULL);
//         fclose(fp);
//         return NULL;
//     }

//     png_init_io(png, fp);
//     png_read_info(png, info);

//     *width = png_get_image_width(png, info);
//     *height = png_get_image_height(png, info);
//     int color_type = png_get_color_type(png, info);
//     int bit_depth = png_get_bit_depth(png, info);

//     // 转换为 RGB 格式
//     if (bit_depth == 16)
//         png_set_strip_16(png);
//     if (color_type == PNG_COLOR_TYPE_PALETTE)
//         png_set_palette_to_rgb(png);
//     if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
//         png_set_expand_gray_1_2_4_to_8(png);
//     if (png_get_valid(png, info, PNG_INFO_tRNS))
//         png_set_tRNS_to_alpha(png);
//     if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
//         png_set_gray_to_rgb(png);

//     png_read_update_info(png, info);

//     unsigned char **row_pointers = (unsigned char **)malloc(sizeof(unsigned char *) * (*height));
//     for (int y = 0; y < *height; y++) {
//         row_pointers[y] = (unsigned char *)malloc(png_get_rowbytes(png, info));
//     }

//     png_read_image(png, row_pointers);

//     unsigned char *rgb_data = (unsigned char *)malloc(*width * *height * 3);
//     for (int y = 0; y < *height; y++) {
//         for (int x = 0; x < *width; x++) {
//             int index = (y * *width + x) * 3;
//             rgb_data[index] = row_pointers[y][x * 3];
//             rgb_data[index + 1] = row_pointers[y][x * 3 + 1];
//             rgb_data[index + 2] = row_pointers[y][x * 3 + 2];
//         }
//         free(row_pointers[y]);
//     }
//     free(row_pointers);

//     png_destroy_read_struct(&png, &info, NULL);
//     fclose(fp);

//     return rgb_data;
// }

// // 根据 PNG 图片生成 GIF
// int create_gif_from_pngs(const char *gif_filename, const char **png_filenames, int num_pngs)
// {
//     int gif_error;
//     GifFileType *gif_file = EGifOpenFileName(gif_filename, false, &gif_error);
//     if (!gif_file) {
//         fprintf(stderr, "Could not open GIF file: %s\n", gif_filename);
//         return -1;
//     }

//     int width, height;
//     unsigned char *rgb_data = read_png_file(png_filenames[0], &width, &height);
//     if (!rgb_data) {
//         EGifCloseFile(gif_file, &gif_error);
//         return -1;
//     }

//     EGifPutScreenDesc(gif_file, width, height, 256, 0, NULL);

//     for (int i = 0; i < num_pngs; i++) {
//         if (i > 0) {
//             free(rgb_data);
//             rgb_data = read_png_file(png_filenames[i], &width, &height);
//             if (!rgb_data) {
//                 EGifCloseFile(gif_file, &gif_error);
//                 return -1;
//             }
//         }

//         EGifPutImageDesc(gif_file, 0, 0, width, height, false, NULL);
//         for (int y = 0; y < height; y++) {
//             EGifPutLine(gif_file, rgb_data + y * width * 3, width * 3);
//         }
//     }

//     free(rgb_data);
//     EGifCloseFile(gif_file, &gif_error);

// return 0;
// }

// #include <gif_lib.h>
// #include <png.h>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <filesystem>
// #include <opencv2/opencv.hpp>

// int main(int argc, char *argv[])
// {

//     std::filesystem::path png_dir = "./pngs/";

//     std::vector<cv::Mat> frames;

//     // 读取 PNG 图片
//     for (int i = 1; i <= 5; ++i) {
//         std::string path = "0" + std::to_string(i) + ".png";

//         auto pngpath = png_dir.string() + path;
//         cv::Mat img = cv::imread(pngpath, cv::IMREAD_UNCHANGED);
//         if (img.empty())
//             continue;
//         frames.push_back(img);
//     }

//     // 创建 GIF

//     auto gifpath = png_dir.string() + "/output.gif";

//     std::cout << "Creating GIF..." << gifpath << std::endl;
//     GifFileType *gif = EGifOpenFileName(gifpath.c_str(), false, nullptr);
//     // 补充颜色量化和帧写入逻辑
//     EGifCloseFile(gif, nullptr);

//     return 0;
// }

// #include <gif_lib.h>
// #include <png.h>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <filesystem>
// #include <unordered_set>
// #include <unordered_map>
// #include <opencv2/opencv.hpp>

// // 颜色结构体（带透明通道）
// struct RGBA
// {
//     uint8_t r;
//     uint8_t g;
//     uint8_t b;
//     uint8_t a;

//     bool operator==(const RGBA &other) const
//     {
//         return r == other.r && g == other.g && b == other.b && a == other.a;
//     }
// };

// namespace std {
// template <>
// struct hash<RGBA>
// {
//     size_t operator()(const RGBA &c) const
//     {
//         return hash<uint32_t>()(*(reinterpret_cast<const uint32_t *>(&c)));
//     }
// };
// } // namespace std

// int main(int argc, char *argv[])
// {
//     std::filesystem::path png_dir = "./pngs/";
//     std::vector<cv::Mat> frames;

//     // 读取PNG图片并验证尺寸
//     for (int i = 1; i <= 5; ++i) {
//         std::string path = "0" + std::to_string(i) + ".png";
//         auto pngpath = png_dir / path;
//         cv::Mat img = cv::imread(pngpath.string(), cv::IMREAD_UNCHANGED);

//         if (img.empty()) {
//             std::cerr << "无法读取图片: " << pngpath << std::endl;
//             continue;
//         }

//         // 统一转换为BGRA格式
//         if (img.channels() == 3) {
//             cv::cvtColor(img, img, cv::COLOR_BGR2BGRA);
//         }

//         frames.push_back(img);
//     }

//     if (frames.empty()) {
//         std::cerr << "没有有效的输入图片" << std::endl;
//         return 1;
//     }

//     // 验证所有帧尺寸一致
//     const cv::Size base_size = frames[0].size();
//     for (const auto &frame : frames) {
//         if (frame.size() != base_size) {
//             std::cerr << "错误：所有图片尺寸必须相同" << std::endl;
//             return 1;
//         }
//     }

//     // 收集颜色并处理透明度
//     std::unordered_set<RGBA> color_set;
//     bool has_transparency = false;

//     for (const auto &frame : frames) {
//         for (int y = 0; y < frame.rows; ++y) {
//             const cv::Vec4b *row = frame.ptr<cv::Vec4b>(y);
//             for (int x = 0; x < frame.cols; ++x) {
//                 const auto &pixel = row[x];
//                 RGBA color{ pixel[2], pixel[1], pixel[0], pixel[3] }; // BGRA -> RGBA

//                 if (color.a < 255) {
//                     has_transparency = true;
//                     if (color.a == 0)
//                         continue; // 完全透明不加入调色板
//                 }

//                 color_set.insert(color);
//             }
//         }
//     }

//     // 创建调色板（最多256色）
//     const int max_colors = has_transparency ? 255 : 256;
//     std::vector<RGBA> palette;

//     if (has_transparency) {
//         palette.push_back({ 0, 0, 0, 0 }); // 透明占位
//     }

//     // 简单颜色裁剪（生产环境建议使用k-means优化）
//     for (const auto &color : color_set) {
//         if (palette.size() >= max_colors)
//             break;
//         palette.push_back(color);
//     }

//     // 创建颜色索引映射
//     std::unordered_map<RGBA, int> color_map;
//     for (size_t i = 0; i < palette.size(); ++i) {
//         color_map[palette[i]] = i;
//     }

//     // 创建GIF文件
//     auto gifpath = png_dir / "output.gif";
//     GifFileType *gif = EGifOpenFileName(gifpath.string().c_str(), false, nullptr);
//     if (!gif) {
//         std::cerr << "无法创建GIF文件" << std::endl;
//         return 1;
//     }

//     // 设置GIF参数
//     ColorMapObject *cmap = GifMakeMapObject(256, nullptr);
//     for (int i = 0; i < 256; ++i) {
//         if (i < palette.size()) {
//             cmap->Colors[i].Red = palette[i].r;
//             cmap->Colors[i].Green = palette[i].g;
//             cmap->Colors[i].Blue = palette[i].b;
//         } else {
//             cmap->Colors[i] = { 0, 0, 0 }; // 填充剩余颜色
//         }
//     }

//     EGifPutScreenDesc(gif, base_size.width, base_size.height, 8, 0, cmap);

//     // 写入帧数据
//     const int delay_time = 10; // 单位：1/100秒
//     for (const auto &frame : frames) {
//         // 图形控制扩展（处理透明）
//         GraphicsControlBlock gcb = { DISPOSE_DO_NOT, 0, delay_time,
//                                      has_transparency ? 0 : NO_TRANSPARENT_COLOR };
//         EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, sizeof(gcb), &gcb);

//         // 准备图像数据
//         GifByteType *gif_image = new GifByteType[base_size.area()];
//         for (int y = 0; y < base_size.height; ++y) {
//             const cv::Vec4b *row = frame.ptr<cv::Vec4b>(y);
//             for (int x = 0; x < base_size.width; ++x) {
//                 const auto &pixel = row[x];
//                 RGBA color{ pixel[2], pixel[1], pixel[0], pixel[3] }; // BGRA -> RGBA

//                 if (color.a == 0) { // 完全透明
//                     gif_image[y * base_size.width + x] = 0;
//                 } else if (color.a < 255) { // 半透明处理
//                     auto it = color_map.find(color);
//                     if (it != color_map.end()) {
//                         gif_image[y * base_size.width + x] = it->second;
//                     } else {
//                         gif_image[y * base_size.width + x] = 0; // 降级为透明
//                     }
//                 } else { // 不透明
//                     auto it = color_map.find(color);
//                     gif_image[y * base_size.width + x] = (it != color_map.end()) ? it->second :
//                     0;
//                 }
//             }
//         }

//         // 写入图像描述
//         EGifPutImageDesc(gif, 0, 0, base_size.width, base_size.height, false, nullptr);
//         for (int y = 0; y < base_size.height; ++y) {
//             EGifPutLine(gif, &gif_image[y * base_size.width], base_size.width);
//         }

//         delete[] gif_image;
//     }

//     EGifCloseFile(gif, nullptr);
//     std::cout << "GIF创建成功: " << gifpath << std::endl;
//     return 0;
// }
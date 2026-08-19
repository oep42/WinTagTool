/* WinTagTool, version 1.0.0 */
#include <string.h>
#include <tag_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


static int import_picture(
    TagLib_File *file,
    const char *image_filename,
    const char *picture_type,
    const char *picture_description,
    int append,
    int open_error,
    int size_error,
    int memory_error,
    int save_error)
{
    FILE *image_file = fopen(image_filename, "rb");

    if (image_file == NULL)
        return open_error;

    fseek(image_file, 0, SEEK_END);
    long image_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    if (image_size <= 0)
    {
        fclose(image_file);
        return size_error;
    }

    char *image_data = malloc(image_size);

    if (image_data == NULL)
    {
        fclose(image_file);
        return memory_error;
    }

    if (fread(image_data, 1, image_size, image_file) != (size_t)image_size)
    {
        free(image_data);
        fclose(image_file);
        return memory_error;
    }

    fclose(image_file);

    const char *mime_type = "image/jpeg";

    const char *extension = strrchr(image_filename, '.');

    if (extension != NULL)
    {
        if (_stricmp(extension, ".png") == 0)
            mime_type = "image/png";
        else if (_stricmp(extension, ".bmp") == 0)
            mime_type = "image/bmp";
        else if (_stricmp(extension, ".gif") == 0)
            mime_type = "image/gif";
        else if (_stricmp(extension, ".webp") == 0)
            mime_type = "image/webp";
        else if (_stricmp(extension, ".avif") == 0)
            mime_type = "image/avif";
    }

    TAGLIB_COMPLEX_PROPERTY_PICTURE(
        picture,
        image_data,
        image_size,
        picture_description,
        mime_type,
        picture_type
    );

    BOOL ok;

    if (append)
        ok = taglib_complex_property_set_append(file, "PICTURE", picture);
    else
        ok = taglib_complex_property_set(file, "PICTURE", picture);

    free(image_data);

    if (!ok)
        return save_error;

    return 0;
}


    static int picture_export_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".ape") == 0 ||
            _stricmp(extension, ".wv") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int picture_count_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".ape") == 0 ||
            _stricmp(extension, ".wv") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int picture_properties_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".ape") == 0 ||
            _stricmp(extension, ".m4a") == 0 ||
            _stricmp(extension, ".mp4") == 0 ||
            _stricmp(extension, ".wv") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int picture_width_height_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".ape") == 0 ||
            _stricmp(extension, ".m4a") == 0 ||
            _stricmp(extension, ".mp4") == 0 ||
            _stricmp(extension, ".wma") == 0 ||
            _stricmp(extension, ".wv") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int tag_count_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".wma") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int tag_value_count_supported(const char *filename)
    {
        const char *extension = strrchr(filename, '.');

        if (extension == NULL)
            return 1;

        if (_stricmp(extension, ".wma") == 0)
        {
            return 0;
        }

        return 1;
    }


    static int debug_picture_properties(
        TagLib_File *file,
        int picture_index)
    {
        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        if (pictures == NULL)
            return 12;

        int count = 0;

        while (pictures[count] != NULL)
            count++;

        if (picture_index < 0 || picture_index >= count)
        {
            taglib_complex_property_free(pictures);
            return 13;
        }

        TagLib_Complex_Property_Attribute **attributes =
            pictures[picture_index];

        int i = 0;

        while (attributes[i] != NULL)
        {
            TagLib_Complex_Property_Attribute *attribute =
                attributes[i];

            printf("key=%s", attribute->key != NULL ? attribute->key : "");

            switch (attribute->value.type)
            {
                case TagLib_Variant_Void:
                    printf(" type=Void");
                    break;

                case TagLib_Variant_Bool:
                    printf(" type=Bool value=%s",
                        attribute->value.value.boolValue ? "true" : "false");
                    break;

                case TagLib_Variant_Int:
                    printf(" type=Int value=%d",
                        attribute->value.value.intValue);
                    break;

                case TagLib_Variant_UInt:
                    printf(" type=UInt value=%u",
                        attribute->value.value.uIntValue);
                    break;

                case TagLib_Variant_LongLong:
                    printf(" type=LongLong value=%lld",
                        attribute->value.value.longLongValue);
                    break;

                case TagLib_Variant_ULongLong:
                    printf(" type=ULongLong value=%llu",
                        attribute->value.value.uLongLongValue);
                    break;

                case TagLib_Variant_Double:
                    printf(" type=Double value=%f",
                        attribute->value.value.doubleValue);
                    break;

                case TagLib_Variant_String:
                    printf(" type=String value=%s",
                        attribute->value.value.stringValue != NULL
                            ? attribute->value.value.stringValue
                            : "");
                    break;

                case TagLib_Variant_StringList:
                    printf(" type=StringList");
                    break;

                case TagLib_Variant_ByteVector:
                    printf(" type=ByteVector size=%u",
                        attribute->value.size);
                    break;

                default:
                    printf(" type=Unknown(%d)",
                        (int)attribute->value.type);
                    break;
            }

            printf("\n");
            i++;
        }

        fflush(stdout);
        taglib_complex_property_free(pictures);

        return 0;
    }


    static int get_jpeg_dimensions(
        const unsigned char *data,
        unsigned int size,
        unsigned int *width,
        unsigned int *height)
    {
        unsigned int pos = 2;

        if (size < 2 ||
            data[0] != 0xFF ||
            data[1] != 0xD8)
        {
            return 1;
        }

        while (pos + 1 < size)
        {
            unsigned int marker;

            while (pos < size && data[pos] != 0xFF)
                pos++;

            if (pos >= size)
                return 1;

            while (pos < size && data[pos] == 0xFF)
                pos++;

            if (pos >= size)
                return 1;

            marker = data[pos++];

            /*
             * Start of Scan. All image data follows this marker.
             * No SOF marker should occur after this point.
             */
            if (marker == 0xDA)
                return 1;

            /*
             * SOF markers containing the image dimensions.
             *
             * C0-C3  Baseline / Extended / Progressive / Lossless
             * C5-C7  Differential versions
             * C9-CB  Arithmetic versions
             * CD-CF  Differential arithmetic versions
             */
            if ((marker >= 0xC0 && marker <= 0xC3) ||
                (marker >= 0xC5 && marker <= 0xC7) ||
                (marker >= 0xC9 && marker <= 0xCB) ||
                (marker >= 0xCD && marker <= 0xCF))
            {
                unsigned int segment_length;

                if (pos + 7 >= size)
                    return 1;

                segment_length =
                    ((unsigned int)data[pos] << 8) |
                    (unsigned int)data[pos + 1];

                if (segment_length < 7 ||
                    pos + segment_length > size)
                {
                    return 1;
                }

                *height =
                    ((unsigned int)data[pos + 3] << 8) |
                    (unsigned int)data[pos + 4];

                *width =
                    ((unsigned int)data[pos + 5] << 8) |
                    (unsigned int)data[pos + 6];

                if (*width == 0 || *height == 0)
                    return 1;

                return 0;
            }

            /*
             * Stand-alone JPEG markers do not have a length field.
             */
            if (marker == 0xD8 ||
                marker == 0xD9 ||
                (marker >= 0xD0 && marker <= 0xD7))
            {
                continue;
            }

            /*
             * All other markers that are not stand-alone have a
             * two-byte length field.
             */
            if (pos + 1 >= size)
                return 1;

            unsigned int segment_length =
                ((unsigned int)data[pos] << 8) |
                (unsigned int)data[pos + 1];

            if (segment_length < 2 ||
                pos + segment_length > size)
            {
                return 1;
            }

            pos += segment_length;
        }

        return 1;
    }


    static int get_png_dimensions(
        const unsigned char *data,
        unsigned int size,
        unsigned int *width,
        unsigned int *height)
    {
        if (size < 24)
            return 1;

        if (data[0] != 0x89 ||
            data[1] != 0x50 ||
            data[2] != 0x4E ||
            data[3] != 0x47 ||
            data[4] != 0x0D ||
            data[5] != 0x0A ||
            data[6] != 0x1A ||
            data[7] != 0x0A)
        {
            return 1;
        }

        /*
         * The first chunk must be IHDR.
         *
         * PNG structure:
         *
         * 8 bytes  signature
         * 4 bytes  chunk length
         * 4 bytes  chunk type
         * 4 bytes  width
         * 4 bytes  height
         */
        if (data[12] != 'I' ||
            data[13] != 'H' ||
            data[14] != 'D' ||
            data[15] != 'R')
        {
            return 1;
        }

        *width =
            ((unsigned int)data[16] << 24) |
            ((unsigned int)data[17] << 16) |
            ((unsigned int)data[18] << 8) |
            (unsigned int)data[19];

        *height =
            ((unsigned int)data[20] << 24) |
            ((unsigned int)data[21] << 16) |
            ((unsigned int)data[22] << 8) |
            (unsigned int)data[23];

        if (*width == 0 || *height == 0)
            return 1;

        return 0;
    }


    static int get_webp_dimensions(
        const unsigned char *data,
        unsigned int size,
        unsigned int *width,
        unsigned int *height)
    {
        if (data == NULL || size < 20)
            return 1;

        /*
         * RIFF / WEBP header
         */
        if (data[0] != 'R' ||
            data[1] != 'I' ||
            data[2] != 'F' ||
            data[3] != 'F' ||
            data[8] != 'W' ||
            data[9] != 'E' ||
            data[10] != 'B' ||
            data[11] != 'P')
        {
            return 1;
        }

        /*
         * VP8X
         *
         * RIFF header:  0..11
         * FourCC:      12..15
         * chunk size:  16..19
         * VP8X data:   20..
         *
         * Canvas width  minus 1: bytes 24..26
         * Canvas height minus 1: bytes 27..29
         */
        if (data[12] == 'V' &&
            data[13] == 'P' &&
            data[14] == '8' &&
            data[15] == 'X')
        {
            if (size < 30)
                return 1;

            *width =
                1 +
                (unsigned int)data[24] |
                ((unsigned int)data[25] << 8) |
                ((unsigned int)data[26] << 16);

            *height =
                1 +
                (unsigned int)data[27] |
                ((unsigned int)data[28] << 8) |
                ((unsigned int)data[29] << 16);

            if (*width == 0 || *height == 0)
                return 1;

            return 0;
        }

        /*
         * VP8 (lossy)
         *
         * VP8 chunk data starts at offset 20.
         *
         * Frame tag:
         *   bytes 20..22
         *
         * Start code:
         *   bytes 23..25 = 9D 01 2A
         *
         * Width:
         *   bytes 26..27
         *
         * Height:
         *   bytes 28..29
         */
        if (data[12] == 'V' &&
            data[13] == 'P' &&
            data[14] == '8' &&
            data[15] == ' ')
        {
            if (size < 30)
                return 1;

            if (data[23] != 0x9D ||
                data[24] != 0x01 ||
                data[25] != 0x2A)
            {
                return 1;
            }

            *width =
                (unsigned int)data[26] |
                ((unsigned int)data[27] << 8);

            *height =
                (unsigned int)data[28] |
                ((unsigned int)data[29] << 8);

            /*
             * The two most significant bits contain scale information.
             * Only the lower 14 bits contain the dimension.
             */
            *width &= 0x3FFF;
            *height &= 0x3FFF;

            if (*width == 0 || *height == 0)
                return 1;

            return 0;
        }

        /*
         * VP8L (lossless)
         *
         * VP8L data starts at offset 20.
         *
         * Signature:
         *   byte 20 = 0x2F
         *
         * Width and height are stored as two consecutive
         * 14-bit little-endian values.
         */
        if (data[12] == 'V' &&
            data[13] == 'P' &&
            data[14] == '8' &&
            data[15] == 'L')
        {
            if (size < 25)
                return 1;

            if (data[20] != 0x2F)
                return 1;

            *width =
                1 +
                ((unsigned int)data[21] |
                 ((unsigned int)(data[22] & 0x3F) << 8));

            *height =
                1 +
                (((unsigned int)data[22] >> 6) |
                 ((unsigned int)data[23] << 2) |
                 ((unsigned int)(data[24] & 0x0F) << 10));

            if (*width == 0 || *height == 0)
                return 1;

            return 0;
        }

        return 1;
    }


    static int get_picture_dimension(
        TagLib_File *file,
        int picture_index,
        const char *dimension)
    {
        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        if (pictures == NULL)
            return 12;

        int count = 0;

        while (pictures[count] != NULL)
            count++;

        if (picture_index < 0 || picture_index >= count)
        {
            taglib_complex_property_free(pictures);
            return 13;
        }

        TagLib_Complex_Property_Attribute **attributes =
            pictures[picture_index];

        const unsigned char *data = NULL;
        unsigned int size = 0;
        const char *mime_type = NULL;

        unsigned int width = 0;
        unsigned int height = 0;

        int taglib_width_found = 0;
        int taglib_height_found = 0;

        int i = 0;

        while (attributes[i] != NULL)
        {
            TagLib_Complex_Property_Attribute *attribute =
                attributes[i];

            if (attribute->key != NULL)
            {
                if (_stricmp(attribute->key, "data") == 0 &&
                    attribute->value.type == TagLib_Variant_ByteVector)
                {
                    data = (const unsigned char *)
                        attribute->value.value.byteVectorValue;

                    size = attribute->value.size;
                }

                else if (_stricmp(attribute->key, "mimeType") == 0 &&
                         attribute->value.type == TagLib_Variant_String)
                {
                    mime_type = attribute->value.value.stringValue;
                }

                else if (_stricmp(attribute->key, "width") == 0 &&
                         attribute->value.type == TagLib_Variant_Int)
                {
                    if (attribute->value.value.intValue > 0)
                    {
                        width =
                            (unsigned int)attribute->value.value.intValue;

                        taglib_width_found = 1;
                    }
                }

                else if (_stricmp(attribute->key, "height") == 0 &&
                         attribute->value.type == TagLib_Variant_Int)
                {
                    if (attribute->value.value.intValue > 0)
                    {
                        height =
                            (unsigned int)attribute->value.value.intValue;

                        taglib_height_found = 1;
                    }
                }
            }

            i++;
        }

        /*
         * Use dimensions supplied directly by TagLib when available.
         */
        if (taglib_width_found && taglib_height_found)
        {
            if (_stricmp(dimension, "width") == 0)
                printf("%u\n", width);

            else if (_stricmp(dimension, "height") == 0)
                printf("%u\n", height);

            else
            {
                taglib_complex_property_free(pictures);
                return 4;
            }

            fflush(stdout);

            taglib_complex_property_free(pictures);

            return 0;
        }

        /*
         * TagLib did not provide usable dimensions.
         * Try to determine them from the embedded image itself.
         */
        int rc = 1;

        if (data != NULL && size > 0 && mime_type != NULL)
        {
            if (_stricmp(mime_type, "image/jpeg") == 0 ||
                _stricmp(mime_type, "image/jpg") == 0)
            {
                rc = get_jpeg_dimensions(
                    data,
                    size,
                    &width,
                    &height
                );
            }

            else if (_stricmp(mime_type, "image/png") == 0)
            {
                rc = get_png_dimensions(
                    data,
                    size,
                    &width,
                    &height
                );
            }

            else if (_stricmp(mime_type, "image/webp") == 0)
            {
                rc = get_webp_dimensions(
                    data,
                    size,
                    &width,
                    &height
                );
            }
        }

        taglib_complex_property_free(pictures);

        if (rc != 0)
            return 20;

        if (_stricmp(dimension, "width") == 0)
            printf("%u\n", width);

        else if (_stricmp(dimension, "height") == 0)
            printf("%u\n", height);

        else
            return 4;

        fflush(stdout);

        return 0;
    }


    static void get_picture_property(
        TagLib_File *file,
        int picture_index,
        int property)
    {
        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        if (pictures == NULL)
            return;

        int count = 0;

        while (pictures[count] != NULL)
            count++;

        if (picture_index >= 0 && picture_index < count)
        {
            TagLib_Complex_Property_Picture_Data picture;

            taglib_picture_from_complex_property(
                &pictures[picture_index],
                &picture
            );

            if (property == 1 && picture.pictureType != NULL)
                printf("%s\n", picture.pictureType);

            else if (property == 2 && picture.description != NULL)
                printf("%s\n", picture.description);

            else if (property == 3 && picture.mimeType != NULL)
                printf("%s\n", picture.mimeType);

            else if (property == 4)
                printf("%u\n", picture.size);

            fflush(stdout);
        }

        taglib_complex_property_free(pictures);
    }


    static int get_audio_property(
        TagLib_File *file,
        const char *property,
        int *value)
    {
        const TagLib_AudioProperties *properties =
            taglib_file_audioproperties(file);

        if (properties == NULL)
            return 7;

        if (strcmp(property, "length") == 0)
            *value = taglib_audioproperties_length(properties);

        else if (strcmp(property, "bitrate") == 0)
            *value = taglib_audioproperties_bitrate(properties);

        else if (strcmp(property, "samplerate") == 0)
            *value = taglib_audioproperties_samplerate(properties);

        else if (strcmp(property, "channels") == 0)
            *value = taglib_audioproperties_channels(properties);

        else
            return 4;

        return 0;
    }



int main(int argc, char *argv[])
{
    TagLib_File *file;
    char *option;
    char *tag;
    char *value;
    char *p;

    if (argc != 3)
        return 1;

    option = argv[1];

    file = taglib_file_new(argv[2]);

    if (file == NULL)
        return 2;


    if (strncmp(option, "--set-tag=", 10) == 0)
    {
        tag = option + 10;

        p = strchr(tag, '=');

        if (p == NULL)
        {
            taglib_file_free(file);
            return 3;
        }

        *p = '\0';
        value = p + 1;

        TagLib_Tag *simple_tag = taglib_file_tag(file);

        if (_stricmp(tag, "TITLE") == 0)
            taglib_tag_set_title(simple_tag, value);

        else if (_stricmp(tag, "ARTIST") == 0)
            taglib_tag_set_artist(simple_tag, value);

        else if (_stricmp(tag, "ALBUM") == 0)
            taglib_tag_set_album(simple_tag, value);

        else if (_stricmp(tag, "COMMENT") == 0)
            taglib_tag_set_comment(simple_tag, value);

        else if (_stricmp(tag, "GENRE") == 0)
            taglib_tag_set_genre(simple_tag, value);

        else if (_stricmp(tag, "YEAR") == 0)
            taglib_tag_set_year(simple_tag,
                (unsigned int)strtoul(value, NULL, 10));

        else if (_stricmp(tag, "TRACK") == 0)
            taglib_tag_set_track(simple_tag,
                (unsigned int)strtoul(value, NULL, 10));

        else
            taglib_property_set(file, tag, value);
    }


    else if (strncmp(option, "--append-tag=", 13) == 0)
    {
        tag = option + 13;

        p = strchr(tag, '=');

        if (p == NULL)
        {
            taglib_file_free(file);
            return 19;
        }

        *p = '\0';
        value = p + 1;

        taglib_property_set_append(file, tag, value);
    }


    else if (strncmp(option, "--get-tag=", 10) == 0)
    {
        tag = option + 10;

        char **values = taglib_property_get(file, tag);

        if (values != NULL)
        {
            int i = 0;

            while (values[i] != NULL)
            {
                printf("%s\n", values[i]);
                i++;
            }

            fflush(stdout);

            taglib_property_free(values);
        }

        taglib_file_free(file);

        return 0;
    }


    else if (strncmp(option, "--remove-tag=", 13) == 0)
    {
        tag = option + 13;

        taglib_property_set(file, tag, NULL);
    }


    else if (strncmp(option, "--has-tag=", 10) == 0)
    {
        tag = option + 10;

        char **values = taglib_property_get(file, tag);

        if (values != NULL && values[0] != NULL)
            printf("true\n");
        else
            printf("false\n");

        fflush(stdout);

        if (values != NULL)
            taglib_property_free(values);

        taglib_file_free(file);

        return 0;
    }


    else if (strncmp(option, "--count-values-of-tag=", 22) == 0)
    {
        if (!tag_value_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        tag = option + 22;

        char **values = taglib_property_get(file, tag);

        int count = 0;

        if (values != NULL)
        {
            while (values[count] != NULL)
                count++;

            taglib_property_free(values);
        }

        printf("%d\n", count);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--count-all-tags") == 0)
    {
        if (!tag_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        char **tags = taglib_property_keys(file);

        int count = 0;

        if (tags != NULL)
        {
            while (tags[count] != NULL)
                count++;

            taglib_property_free(tags);
        }

        printf("%d\n", count);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--count-all-tag-values") == 0)
    {
        if (!tag_value_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        char **tags = taglib_property_keys(file);

        int count = 0;

        if (tags != NULL)
        {
            int i = 0;

            while (tags[i] != NULL)
            {
                char **values = taglib_property_get(file, tags[i]);

                if (values != NULL)
                {
                    int j = 0;

                    while (values[j] != NULL)
                    {
                        count++;
                        j++;
                    }

                    taglib_property_free(values);
                }

                i++;
            }

            taglib_property_free(tags);
        }

        printf("%d\n", count);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--list-tags") == 0)
    {
        char **tags = taglib_property_keys(file);

        if (tags != NULL)
        {
            int i = 0;

            while (tags[i] != NULL)
            {
                printf("%s\n", tags[i]);
                i++;
            }

            fflush(stdout);

            taglib_property_free(tags);
        }

        taglib_file_free(file);

        return 0;
    }


    else if (strcmp(option, "--list-tags-and-values") == 0)
    {
        char **tags = taglib_property_keys(file);

        if (tags != NULL)
        {
            int i = 0;

            while (tags[i] != NULL)
            {
                char **values = taglib_property_get(file, tags[i]);

                if (values != NULL)
                {
                    int j = 0;

                    while (values[j] != NULL)
                    {
                        printf("%s=%s\n", tags[i], values[j]);
                        j++;
                    }

                    taglib_property_free(values);
                }

                i++;
            }

            taglib_property_free(tags);
        }

        fflush(stdout);

        taglib_file_free(file);

        return 0;
    }


    else if (strncmp(option, "--import-picture-from=", 22) == 0)
    {
        char *image_filename = option + 22;

        const char *picture_type = "Front Cover";
        const char *picture_description = "";

        char *p = strchr(image_filename, '=');

        if (p != NULL)
        {
            *p = '\0';

            picture_type = p + 1;

            char *q = strchr(picture_type, '=');

            if (q != NULL)
            {
                *q = '\0';
                picture_description = q + 1;
            }
        }

        int rc = import_picture(
            file,
            image_filename,
            picture_type,
            picture_description,
            0,
            7,
            8,
            9,
            10);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }
    }


    else if (strncmp(option, "--append-picture-from=", 22) == 0)
    {
        char *image_filename = option + 22;

        const char *picture_type = "Other";
        const char *picture_description = "";

        char *p = strchr(image_filename, '=');

        if (p != NULL)
        {
            *p = '\0';

            picture_type = p + 1;

            char *q = strchr(picture_type, '=');

            if (q != NULL)
            {
                *q = '\0';
                picture_description = q + 1;
            }
        }

        int rc = import_picture(
            file,
            image_filename,
            picture_type,
            picture_description,
            1,
            15,
            16,
            17,
            18);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }
    }


    else if (strncmp(option, "--export-picture=", 17) == 0)
    {
        if (!picture_export_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        char *p = option + 17;
        char *q = strchr(p, '=');

        if (q == NULL)
        {
            taglib_file_free(file);
            return 11;
        }

        *q = '\0';

        int picture_index = atoi(p);

        const char *base_filename = q + 1;

        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        if (pictures == NULL)
        {
            taglib_file_free(file);
            return 12;
        }

        int count = 0;

        while (pictures[count] != NULL)
            count++;

        if (picture_index < 0 || picture_index >= count)
        {
            taglib_complex_property_free(pictures);
            taglib_file_free(file);
            return 13;
        }

        TagLib_Complex_Property_Picture_Data picture;

        taglib_picture_from_complex_property(
            &pictures[picture_index],
            &picture
        );

        const char *extension = ".bin";

        if (picture.mimeType != NULL)
        {
            if (_stricmp(picture.mimeType, "image/jpeg") == 0)
                extension = ".jpg";
            else if (_stricmp(picture.mimeType, "image/png") == 0)
                extension = ".png";
            else if (_stricmp(picture.mimeType, "image/bmp") == 0)
                extension = ".bmp";
            else if (_stricmp(picture.mimeType, "image/gif") == 0)
                extension = ".gif";
            else if (_stricmp(picture.mimeType, "image/webp") == 0)
                extension = ".webp";
            else if (_stricmp(picture.mimeType, "image/avif") == 0)
                extension = ".avif";
        }

        char filename[MAX_PATH];

        snprintf(
            filename,
            sizeof(filename),
            "%s%03d%s",
            base_filename,
            picture_index,
            extension
        );

        FILE *fp = fopen(filename, "wb");

        if (fp == NULL)
        {
            taglib_complex_property_free(pictures);
            taglib_file_free(file);
            return 14;
        }

        fwrite(picture.data, 1, picture.size, fp);

        fclose(fp);

        taglib_complex_property_free(pictures);

        printf("%s\n", filename);
        fflush(stdout);

        taglib_file_free(file);

        return 0;
    }


    else if (strcmp(option, "--remove-pictures") == 0)
    {
        if (!taglib_complex_property_set(file, "PICTURE", NULL))
        {
            taglib_file_free(file);
            return 6;
        }
    }


    else if (strncmp(option, "--has-picture=", 14) == 0)
    {
        if (!picture_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        int picture_index = atoi(option + 14);

        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        int exists = 0;

        if (pictures != NULL)
        {
            int count = 0;

            while (pictures[count] != NULL)
                count++;

            if (picture_index >= 0 && picture_index < count)
                exists = 1;

            taglib_complex_property_free(pictures);
        }

        if (exists)
            printf("true\n");
        else
            printf("false\n");

        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--count-pictures") == 0)
    {
        if (!picture_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        int count = 0;

        TagLib_Complex_Property_Attribute ***pictures =
            taglib_complex_property_get(file, "PICTURE");

        if (pictures != NULL)
        {
            while (pictures[count] != NULL)
                count++;

            taglib_complex_property_free(pictures);
        }

        printf("%d\n", count);
        fflush(stdout);
        
        taglib_file_free(file);

        return 0;
    }


    else if (strncmp(option, "--get-picture-type=", 19) == 0)
    {
        if (!picture_properties_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        get_picture_property(
            file,
            atoi(option + 19),
            1
        );

        taglib_file_free(file);
        return 0;
    }


    else if (strncmp(option, "--get-picture-description=", 26) == 0)
    {
        if (!picture_properties_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        get_picture_property(
            file,
            atoi(option + 26),
            2
        );

        taglib_file_free(file);
        return 0;
    }


    else if (strncmp(option, "--get-picture-mime-type=", 24) == 0)
    {
        if (!picture_properties_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        get_picture_property(
            file,
            atoi(option + 24),
            3
        );

        taglib_file_free(file);
        return 0;
    }


    else if (strncmp(option, "--get-picture-size=", 19) == 0)
    {
        if (!picture_count_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        get_picture_property(
            file,
            atoi(option + 19),
            4
        );

        taglib_file_free(file);
        return 0;
    }


    else if (strncmp(option, "--debug-picture-properties=", 27) == 0)
    {
        int rc = debug_picture_properties(
            file,
            atoi(option + 27)
        );

        taglib_file_free(file);
        return rc;
    }


    else if (strncmp(option, "--get-picture-width=", 20) == 0)
    {
        if (!picture_width_height_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        int rc = get_picture_dimension(
            file,
            atoi(option + 20),
            "width"
        );

        taglib_file_free(file);
        return rc;
    }


    else if (strncmp(option, "--get-picture-height=", 21) == 0)
    {
        if (!picture_width_height_supported(argv[2]))
        {
            taglib_file_free(file);
            return 20;
        }

        int rc = get_picture_dimension(
            file,
            atoi(option + 21),
            "height"
        );

        taglib_file_free(file);
        return rc;
    }


    else if (strcmp(option, "--get-audio-length") == 0)
    {
        int value;
        int rc = get_audio_property(file, "length", &value);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }

        printf("%d\n", value);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--get-audio-bitrate") == 0)
    {
        int value;
        int rc = get_audio_property(file, "bitrate", &value);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }

        printf("%d\n", value);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--get-audio-samplerate") == 0)
    {
        int value;
        int rc = get_audio_property(file, "samplerate", &value);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }

        printf("%d\n", value);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else if (strcmp(option, "--count-audio-channels") == 0)
    {
        int value;
        int rc = get_audio_property(file, "channels", &value);

        if (rc != 0)
        {
            taglib_file_free(file);
            return rc;
        }

        printf("%d\n", value);
        fflush(stdout);

        taglib_file_free(file);
        return 0;
    }


    else
    {
        taglib_file_free(file);
        return 4;
    }

    if (!taglib_file_save(file))
    {
        taglib_file_free(file);
        return 5;
    }

    taglib_file_free(file);

    return 0;
}

#include "tests.h"

int my_strlen(char *str) {
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void my_strcat(char *str_1, char *str_2) {
    while (*str_1 != '\0'){str_1++;}
    while (*str_2 != '\0')
    {
        *str_1 = *str_2;
        str_1++;
        str_2++;
    }
    *str_1 = '\0';
}

char* my_strstr(char *s, char *p) {
    int p_len = 0; int s_len = 0;

    while (p[p_len] != '\0') p_len++;
    while (s[s_len] != '\0')
    {
        int i;
        for (i = 0; i < p_len; i++)
        {
            if (s[s_len + i] != p[i]) { break; }
        }
        if (i == p_len) { return &s[s_len]; }
        s_len++;
    }
    return 0;
}

void rgb2gray(float *in, float *out, int h, int w) {
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float r = in[(y * w + x) * 3 + 0];
            float g = in[(y * w + x) * 3 + 1];
            float b = in[(y * w + x) * 3 + 2];

            float gray = 0.1140 * b + 0.5870 * g + 0.2989 * r;

            out[y * w + x] = gray;
        }
    }
}

void resize(float *in, float *out, int h, int w, int c, float scale) {
    int new_h = static_cast<int>(h * scale);
    int new_w = static_cast<int>(w * scale);

    for (int y = 0; y < new_h; y++)
    {
        for (int x = 0; x < new_w; x++)
        {
            float x0 = x / scale;
            float y0 = y / scale;

            int x1 = static_cast<int>(x0);
            int y1 = static_cast<int>(y0);

            x1 = 0 > (x1 < w - 1 ? x1 : w - 1) ? 0 : (x1 < w - 1 ? x1 : w - 1);
            y1 = 0 > (y1 < h - 1 ? y1 : h - 1) ? 0 : (y1 < h - 1 ? y1 : h - 1);

            int x2 = x1 + 1 < w - 1 ? x1 + 1 : w - 1;
            int y2 = y1 + 1 < h - 1 ? y1 + 1 : h - 1;

            float dx = x0 - x1;
            float dy = y0 - y1;

            for (int i = 0; i < c; i++)
            {
            float p1 = in[(y1 * w + x1) * c + i];
            float p2 = in[(y1 * w + x2) * c + i];
            float p3 = in[(y2 * w + x1) * c + i];
            float p4 = in[(y2 * w + x2) * c + i];

            float q = p1 * (1 - dx) * (1 - dy) + p2 * dx * (1 - dy) +
                p3 * (1 - dx) * dy + p4 * dx * dy;

            out[(y * new_w + x) *c + i] = q;
            }
        }
    }

}

void hist_eq(float *in, int h, int w) {
    int histogram[256] = { 0 };

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int pixel_value = (int)(in[i * w + j]);
            histogram[pixel_value]++;
        }
    }

    
    float cdf[256] = { 0 };
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

  
    float cdf_min = cdf[0];
    float scale = 255.0f / (h * w - cdf_min);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int pixel_value = (int)(in[i * w + j]);
            in[i * w + j] = (cdf[pixel_value] - cdf_min) * scale;
        }
    }
}
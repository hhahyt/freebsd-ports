https://git.libav.org/?p=libav.git;a=commitdiff;h=c6558e8840fb

--- libavcodec/libx264.c.orig	2017-05-14 19:25:01 UTC
+++ libavcodec/libx264.c
@@ -243,7 +243,11 @@ static int X264_frame(AVCodecContext *ctx, AVPacket *p
 
     x264_picture_init( &x4->pic );
     x4->pic.img.i_csp   = x4->params.i_csp;
+#if X264_BUILD >= 153
+    if (x4->params.i_bitdepth > 8)
+#else
     if (x264_bit_depth > 8)
+#endif
         x4->pic.img.i_csp |= X264_CSP_HIGH_DEPTH;
     x4->pic.img.i_plane = 3;
 
@@ -395,6 +399,9 @@ static av_cold int X264_init(AVCodecContext *avctx)
     x4->params.p_log_private        = avctx;
     x4->params.i_log_level          = X264_LOG_DEBUG;
     x4->params.i_csp                = convert_pix_fmt(avctx->pix_fmt);
+#if X264_BUILD >= 153
+    x4->params.i_bitdepth           = av_pix_fmt_desc_get(avctx->pix_fmt)->comp[0].depth;
+#endif
 
     if (avctx->bit_rate) {
         x4->params.rc.i_bitrate   = avctx->bit_rate / 1000;
@@ -675,6 +682,7 @@ FF_ENABLE_DEPRECATION_WARNINGS
     return 0;
 }
 
+#if X264_BUILD < 153
 static const enum AVPixelFormat pix_fmts_8bit[] = {
     AV_PIX_FMT_YUV420P,
     AV_PIX_FMT_YUVJ420P,
@@ -701,15 +709,37 @@ static const enum AVPixelFormat pix_fmts_10bit[] = {
     AV_PIX_FMT_NV20,
     AV_PIX_FMT_NONE
 };
+#else
+static const enum AVPixelFormat pix_fmts_all[] = {
+    AV_PIX_FMT_YUV420P,
+    AV_PIX_FMT_YUVJ420P,
+    AV_PIX_FMT_YUV422P,
+    AV_PIX_FMT_YUVJ422P,
+    AV_PIX_FMT_YUV444P,
+    AV_PIX_FMT_YUVJ444P,
+    AV_PIX_FMT_NV12,
+    AV_PIX_FMT_NV16,
+    AV_PIX_FMT_NV21,
+    AV_PIX_FMT_YUV420P10,
+    AV_PIX_FMT_YUV422P10,
+    AV_PIX_FMT_YUV444P10,
+    AV_PIX_FMT_NV20,
+    AV_PIX_FMT_NONE
+};
+#endif
 
 static av_cold void X264_init_static(AVCodec *codec)
 {
+#if X264_BUILD < 153
     if (x264_bit_depth == 8)
         codec->pix_fmts = pix_fmts_8bit;
     else if (x264_bit_depth == 9)
         codec->pix_fmts = pix_fmts_9bit;
     else if (x264_bit_depth == 10)
         codec->pix_fmts = pix_fmts_10bit;
+#else
+    codec->pix_fmts = pix_fmts_all;
+#endif
 }
 
 #define OFFSET(x) offsetof(X264Context, x)

/*
 * Copyright (c) 2020, 2021 Oracle and/or its affiliates. All rights reserved. This
 * code is released under a tri EPL/GPL/LGPL license. You can use it,
 * redistribute it and/or modify it under the terms of the:
 *
 * Eclipse Public License version 2.0, or
 * GNU General Public License version 2, or
 * GNU Lesser General Public License version 2.1.
 */
#include <truffleruby-impl.h>
#include <ruby/encoding.h>

// Encoding, rb_enc_*

POLYGLOT_DECLARE_TYPE(rb_encoding)

// returns Truffle::CExt::RbEncoding, takes Encoding or String
rb_encoding* rb_to_encoding(VALUE encoding) {
  encoding = RUBY_CEXT_INVOKE("rb_convert_to_encoding", encoding); // Convert to Encoding
  return polyglot_as_rb_encoding(RUBY_CEXT_INVOKE_NO_WRAP("rb_to_encoding", encoding));
}

rb_encoding* rb_encoding_to_native(char* name) {
  OnigEncodingType* native = calloc(1, sizeof(rb_encoding)); // calloc() to zero-fill
  native->name = name;
  return native;
}

rb_encoding* rb_default_external_encoding(void) {
  VALUE result = RUBY_CEXT_INVOKE("rb_default_external_encoding");
  if (NIL_P(result)) {
    return NULL;
  }
  return rb_to_encoding(result);
}

rb_encoding* rb_default_internal_encoding(void) {
  VALUE result = RUBY_CEXT_INVOKE("rb_default_internal_encoding");
  if (NIL_P(result)) {
    return NULL;
  }
  return rb_to_encoding(result);
}

rb_encoding* rb_locale_encoding(void) {
  VALUE result = RUBY_CEXT_INVOKE("rb_locale_encoding");
  if (NIL_P(result)) {
    return NULL;
  }
  return rb_to_encoding(result);
}

int rb_locale_encindex(void) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_locale_encindex"));
}

rb_encoding* rb_filesystem_encoding(void) {
  VALUE result = RUBY_CEXT_INVOKE("rb_filesystem_encoding");
  if (NIL_P(result)) {
    return NULL;
  }
  return rb_to_encoding(result);
}

int rb_filesystem_encindex(void) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_filesystem_encindex"));
}

rb_encoding* get_encoding(VALUE string) {
  return rb_to_encoding(RUBY_INVOKE(string, "encoding"));
}

unsigned int rb_enc_codepoint_len(const char *p, const char *e, int *len_p, rb_encoding *encoding) {
  int len = e - p;
  if (len <= 0) {
    rb_raise(rb_eArgError, "empty string");
  }
  VALUE array = RUBY_CEXT_INVOKE("rb_enc_codepoint_len", rb_str_new(p, len), rb_enc_from_encoding(encoding));
  if (len_p) *len_p = polyglot_as_i32(polyglot_invoke(rb_tr_unwrap(array), "[]", 0));
  return (unsigned int)polyglot_as_i32(polyglot_invoke(rb_tr_unwrap(array), "[]", 1));
}

int rb_enc_mbc_to_codepoint(char *p, char *e, rb_encoding *enc) {
  int length = e - p;
  return polyglot_as_i32(polyglot_invoke(RUBY_CEXT, "rb_enc_mbc_to_codepoint",
      rb_tr_unwrap(rb_enc_from_encoding(enc)),
      rb_tr_unwrap(rb_str_new(p, length)),
      length));
}

int rb_tr_code_to_mbclen(OnigCodePoint code, OnigEncodingType *encoding) {
  return polyglot_as_i32(polyglot_invoke(RUBY_CEXT, "code_to_mbclen", code, rb_tr_unwrap(rb_enc_from_encoding(encoding))));
}

int rb_enc_codelen(int c, rb_encoding *enc) {
  int n = ONIGENC_CODE_TO_MBCLEN(enc,c);
  if (n == 0) {
    rb_raise(rb_eArgError, "invalid codepoint 0x%x in %s", c, rb_enc_name(enc));
  }
  return n;
}

rb_encoding* rb_enc_get(VALUE object) {
  return rb_to_encoding(RUBY_CEXT_INVOKE("rb_enc_get", object));
}

void rb_enc_set_index(VALUE obj, int idx) {
  polyglot_invoke(RUBY_CEXT, "rb_enc_set_index", rb_tr_unwrap(obj), idx);
}

rb_encoding* rb_ascii8bit_encoding(void) {
  return rb_to_encoding(RUBY_CEXT_INVOKE("ascii8bit_encoding"));
}

int rb_ascii8bit_encindex(void) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_ascii8bit_encindex"));
}

rb_encoding* rb_usascii_encoding(void) {
  return rb_to_encoding(RUBY_CEXT_INVOKE("usascii_encoding"));
}

int rb_enc_asciicompat(rb_encoding *enc) {
  return polyglot_as_boolean(RUBY_INVOKE_NO_WRAP(rb_enc_from_encoding(enc), "ascii_compatible?"));
}

void rb_must_asciicompat(VALUE str) {
  rb_encoding *enc = rb_enc_get(str);
  if (!rb_enc_asciicompat(enc)) {
    rb_raise(rb_eEncCompatError, "ASCII incompatible encoding: %s", rb_enc_name(enc));
  }
}

int rb_usascii_encindex(void) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_usascii_encindex"));
}

rb_encoding* rb_utf8_encoding(void) {
  return rb_to_encoding(RUBY_CEXT_INVOKE("utf8_encoding"));
}

int rb_utf8_encindex(void) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_utf8_encindex"));
}

enum ruby_coderange_type RB_ENC_CODERANGE(VALUE obj) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("RB_ENC_CODERANGE", obj));
}

void rb_enc_coderange_clear(VALUE obj) {
  RUBY_CEXT_INVOKE("rb_enc_coderange_clear", obj);
}

VALUE rb_enc_associate(VALUE obj, rb_encoding *enc) {
  return rb_enc_associate_index(obj, rb_enc_to_index(enc));
}

VALUE rb_enc_associate_index(VALUE obj, int idx) {
  return rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_enc_associate_index", rb_tr_unwrap(obj), idx));
}

rb_encoding* rb_enc_compatible(VALUE str1, VALUE str2) {
  VALUE result = RUBY_INVOKE(rb_cEncoding, "compatible?", str1, str2);
  if (!NIL_P(result)) {
    return rb_to_encoding(result);
  }
  return NULL;
}

void rb_enc_copy(VALUE obj1, VALUE obj2) {
  rb_enc_associate_index(obj1, rb_enc_get_index(obj2));
}

int rb_enc_find_index(const char *name) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_find_index", rb_str_new_cstr(name)));
}

rb_encoding* rb_enc_find(const char *name) {
  int idx = rb_enc_find_index(name);
  if (idx < 0) idx = 0;
  return rb_enc_from_index(idx);
}

int rb_enc_isalnum(unsigned char c, rb_encoding *enc) {
  return polyglot_as_boolean(polyglot_invoke(RUBY_CEXT, "rb_enc_isalnum", c, rb_tr_unwrap(rb_enc_from_encoding(enc))));
}

int rb_enc_isspace(unsigned char c, rb_encoding *enc) {
  return polyglot_as_boolean(polyglot_invoke(RUBY_CEXT, "rb_enc_isspace", c, rb_tr_unwrap(rb_enc_from_encoding(enc))));
}

// returns Encoding, takes rb_encoding struct or RbEncoding
VALUE rb_enc_from_encoding(rb_encoding *encoding) {
  if (polyglot_is_value(encoding)) {
    return rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_enc_from_encoding", encoding));
  } else {
    return rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_enc_from_native_encoding", (long)encoding));
  }
}

rb_encoding* rb_enc_from_index(int index) {
  return rb_to_encoding(rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_enc_from_index", index)));
}

int rb_enc_str_coderange(VALUE str) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_str_coderange", str));
}

int rb_enc_to_index(rb_encoding *enc) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_to_index", rb_enc_from_encoding(enc)));
}

VALUE rb_obj_encoding(VALUE obj) {
  return RUBY_INVOKE(obj, "encoding");
}

int rb_to_encoding_index(VALUE enc) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_to_encoding_index", enc));
}

int rb_enc_get_index(VALUE obj) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_get_index", obj));
}

char* rb_enc_left_char_head(char *start, char *p, char *end, rb_encoding *enc) {
  int length = start-end;
  int position = polyglot_as_i32(polyglot_invoke(RUBY_CEXT, "rb_enc_left_char_head",
      rb_tr_unwrap(rb_enc_from_encoding(enc)),
      rb_tr_unwrap(rb_str_new(start, length)),
      0,
      p-start,
      length));
  return start+position;
}

int rb_enc_precise_mbclen(const char *p, const char *e, rb_encoding *enc) {
  int length = e - p;
  if (e <= p) {
    return ONIGENC_CONSTRUCT_MBCLEN_NEEDMORE(1);
  }
  return polyglot_as_i32(polyglot_invoke(RUBY_CEXT, "rb_enc_precise_mbclen",
      rb_tr_unwrap(rb_enc_from_encoding(enc)),
      rb_tr_unwrap(rb_str_new(p, length)),
      0,
      length));
}

int rb_enc_dummy_p(rb_encoding *enc) {
  return polyglot_as_i32(RUBY_INVOKE_NO_WRAP(rb_enc_from_encoding(enc), "dummy?"));
}

int rb_enc_mbmaxlen(rb_encoding *enc) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_mbmaxlen", rb_enc_from_encoding(enc)));
}

int rb_enc_mbminlen(rb_encoding *enc) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_enc_mbminlen", rb_enc_from_encoding(enc)));
}

int rb_enc_mbclen(const char *p, const char *e, rb_encoding *enc) {
  int length = e-p;
  return polyglot_as_i32(polyglot_invoke(RUBY_CEXT, "rb_enc_mbclen",
      rb_tr_unwrap(rb_enc_from_encoding(enc)),
      rb_tr_unwrap(rb_str_new(p, length)),
      0,
      length));
}

int rb_define_dummy_encoding(const char *name) {
  return polyglot_as_i32(RUBY_CEXT_INVOKE_NO_WRAP("rb_define_dummy_encoding", rb_str_new_cstr(name)));
}

#undef rb_enc_code_to_mbclen
int rb_enc_str_asciionly_p(VALUE str) {
  return polyglot_as_boolean(RUBY_INVOKE_NO_WRAP(str, "ascii_only?"));
}

#undef rb_enc_str_new
VALUE rb_enc_str_new(const char *ptr, long len, rb_encoding *enc) {
  return RUBY_INVOKE(rb_str_new(ptr, len), "force_encoding", rb_enc_from_encoding(enc));
}

#undef rb_enc_str_new_cstr
VALUE rb_enc_str_new_cstr(const char *ptr, rb_encoding *enc) {
  if (rb_enc_mbminlen(enc) != 1) {
    rb_raise(rb_eArgError, "wchar encoding given");
  }

  VALUE string = rb_str_new_cstr(ptr);
  rb_enc_associate(string, enc);
  return string;
}

VALUE rb_enc_str_new_static(const char *ptr, long len, rb_encoding *enc) {
  if (len < 0) {
    rb_raise(rb_eArgError, "negative string size (or size too big)");
  }
  if (!enc) {
    enc = rb_ascii8bit_encoding();
  }

  VALUE string = rb_enc_str_new(ptr, len, enc);
  return string;
}

void rb_enc_raise(rb_encoding *enc, VALUE exc, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  VALUE mesg = rb_vsprintf(fmt, args);
  va_end(args);
  rb_exc_raise(rb_exc_new_str(exc, RUBY_INVOKE(mesg, "force_encoding", rb_enc_from_encoding(enc))));
}

#define castchar(from) (char)((from) & 0xff)

int rb_uv_to_utf8(char buf[6], unsigned long uv) {
  if (uv <= 0x7f) {
    buf[0] = (char)uv;
    return 1;
  }
  if (uv <= 0x7ff) {
    buf[0] = castchar(((uv>>6)&0xff)|0xc0);
    buf[1] = castchar((uv&0x3f)|0x80);
    return 2;
  }
  if (uv <= 0xffff) {
    buf[0] = castchar(((uv>>12)&0xff)|0xe0);
    buf[1] = castchar(((uv>>6)&0x3f)|0x80);
    buf[2] = castchar((uv&0x3f)|0x80);
    return 3;
  }
  if (uv <= 0x1fffff) {
    buf[0] = castchar(((uv>>18)&0xff)|0xf0);
    buf[1] = castchar(((uv>>12)&0x3f)|0x80);
    buf[2] = castchar(((uv>>6)&0x3f)|0x80);
    buf[3] = castchar((uv&0x3f)|0x80);
    return 4;
  }
  if (uv <= 0x3ffffff) {
    buf[0] = castchar(((uv>>24)&0xff)|0xf8);
    buf[1] = castchar(((uv>>18)&0x3f)|0x80);
    buf[2] = castchar(((uv>>12)&0x3f)|0x80);
    buf[3] = castchar(((uv>>6)&0x3f)|0x80);
    buf[4] = castchar((uv&0x3f)|0x80);
    return 5;
  }
  if (uv <= 0x7fffffff) {
    buf[0] = castchar(((uv>>30)&0xff)|0xfc);
    buf[1] = castchar(((uv>>24)&0x3f)|0x80);
    buf[2] = castchar(((uv>>18)&0x3f)|0x80);
    buf[3] = castchar(((uv>>12)&0x3f)|0x80);
    buf[4] = castchar(((uv>>6)&0x3f)|0x80);
    buf[5] = castchar((uv&0x3f)|0x80);
    return 6;
  }

  rb_raise(rb_eRangeError, "pack(U): value out of range");
}

void write_p(const UChar** p, int offset) {
  *p = *p + offset;
}

int rb_tr_enc_mbc_case_fold(rb_encoding *enc, int flag, const UChar** p, const UChar* end, UChar* result) {
  int length = end - *p;
  VALUE result_str = rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_tr_enc_mbc_case_fold",
          rb_tr_unwrap(rb_enc_from_encoding(enc)),
          flag,
          rb_tr_unwrap(rb_str_new((char *)*p, length)),
          write_p,
          p));
   int result_len = RSTRING_LEN(result_str);
   if (result_len > 0) {
     memcpy(result, RSTRING_PTR(result_str), result_len);
   }
   return result_len;
}

int rb_tr_code_to_mbc(OnigCodePoint code, UChar *buf, OnigEncoding enc) {
    VALUE result_str = rb_tr_wrap(polyglot_invoke(RUBY_CEXT, "rb_tr_code_to_mbc",
            rb_tr_unwrap(rb_enc_from_encoding(enc)), code));
   int result_len = RSTRING_LEN(result_str);
   if (result_len > 0) {
     memcpy(buf, RSTRING_PTR(result_str), result_len);
   }
   return result_len;
}

#define ARG_ENCODING_FIXED    16
#define ARG_ENCODING_NONE     32

static int char_to_option(int c) {
  int val;

  switch (c) {
    case 'i':
      val = ONIG_OPTION_IGNORECASE;
      break;
    case 'x':
      val = ONIG_OPTION_EXTEND;
      break;
    case 'm':
      val = ONIG_OPTION_MULTILINE;
      break;
    default:
      val = 0;
      break;
  }
  return val;
}

extern int rb_char_to_option_kcode(int c, int *option, int *kcode) {
  *option = 0;

  switch (c) {
    case 'n':
      *kcode = rb_ascii8bit_encindex();
      return (*option = ARG_ENCODING_NONE);
    case 'e':
      *kcode = rb_enc_find_index("EUC-JP");
      break;
    case 's':
      *kcode = rb_enc_find_index("Windows-31J");
      break;
    case 'u':
      *kcode = rb_utf8_encindex();
      break;
    default:
      *kcode = -1;
      return (*option = char_to_option(c));
  }
  *option = ARG_ENCODING_FIXED;
  return 1;
}

int enc_is_unicode(const OnigEncodingType *enc) {
  const char *name = rb_enc_name(enc);
  return !strncmp(name,"UTF", 3);
}



import sys
import gtk

import re

def replace_many(src2dest, buf):
    src_re = re.compile('|'.join(re.escape(word) for word in src2dest))

    def replace_repl(mo):
        return src2dest[mo.group()]
    return src_re.sub(replace_repl, buf)

if __name__ == '__main__':
    pixbuf64 = gtk.gdk.pixbuf_new_from_file("data/icons/hicolor/64x64/apps/skype.png")
    pixbuf16 = gtk.gdk.pixbuf_new_from_file("data/icons/hicolor/16x16/apps/skype.png")
    src2dest = {'@PACKAGE_VERSION@': sys.argv[1],
                '@DESKTOP_FILE_DIR@': sys.argv[2],
                '@IMAGEDATA64@': ("gtk.gdk.pixbuf_new_from_data(%r, gtk.gdk.COLORSPACE_RGB, %r, %r, %r, %r, %r)" %
                                  (pixbuf64.get_pixels(), pixbuf64.get_has_alpha(), pixbuf64.get_bits_per_sample(),
                                   pixbuf64.get_width(), pixbuf64.get_height(), pixbuf64.get_rowstride())),
                '@IMAGEDATA16@': ("gtk.gdk.pixbuf_new_from_data(%r, gtk.gdk.COLORSPACE_RGB, %r, %r, %r, %r, %r)" %
                                  (pixbuf16.get_pixels(), pixbuf16.get_has_alpha(), pixbuf16.get_bits_per_sample(),
                                   pixbuf16.get_width(), pixbuf16.get_height(), pixbuf16.get_rowstride())),
                }

    buf = sys.stdin.read()
    sys.stdout.write(replace_many(src2dest, buf))

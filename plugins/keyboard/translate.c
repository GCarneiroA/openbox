#include "../../kernel/openbox.h"
#include <glib.h>
#include <string.h>

static guint translate_modifier(char *str)
{
    if (!strcmp("Mod1", str) || !strcmp("A", str)) return Mod1Mask;
    else if (!strcmp("Mod2", str)) return Mod2Mask;
    else if (!strcmp("Mod3", str)) return Mod3Mask;
    else if (!strcmp("Mod4", str) || !strcmp("W", str)) return Mod4Mask;
    else if (!strcmp("Mod5", str)) return Mod5Mask;
    else if (!strcmp("Control", str) || !strcmp("C", str)) return ControlMask;
    else if (!strcmp("Shift", str) || !strcmp("S", str)) return ShiftMask;
    g_warning("Invalid modifier '%s' in binding.", str);
    return 0;
}

gboolean translate_key(char *str, guint *state, guint *keycode)
{
    char **parsed;
    char *l;
    int i;
    gboolean ret = FALSE;
    KeySym sym;

    parsed = g_strsplit(str, "-", -1);
    
    /* first, find the key (last token) */
    l = NULL;
    for (i = 0; parsed[i] != NULL; ++i)
	l = parsed[i];
    if (l == NULL)
	goto translation_fail;

    /* figure out the mod mask */
    *state = 0;
    for (i = 0; parsed[i] != l; ++i) {
	guint m = translate_modifier(parsed[i]);
	if (!m) goto translation_fail;
	*state |= m;
    }

    /* figure out the keycode */
    sym = XStringToKeysym(l);
    if (sym == NoSymbol) {
	g_warning("Invalid key name '%s' in key binding.", l);
	goto translation_fail;
    }
    *keycode = XKeysymToKeycode(ob_display, sym);
    if (!keycode) {
	g_warning("Key '%s' does not exist on the display.", l); 
	goto translation_fail;
    }

    ret = TRUE;

translation_fail:
    g_strfreev(parsed);
    return ret;
}

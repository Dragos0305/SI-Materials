/*
 * stack_visualizer.c
 *
 * A small teaching tool for the SI (Securitatea Informatiei) course.
 *
 * It reads a string from the keyboard into a fixed-size stack buffer and
 * then draws an ASCII-art picture of what the stack frame looks like right
 * after the read. The goal is to make it obvious:
 *
 *   - where the buffer lives on the stack,
 *   - how the bytes you typed fill it (and how the NUL terminator sits),
 *   - what "important" slots sit just above the buffer (saved base pointer,
 *     a fake "canary", and the saved return address),
 *   - and, crucially, what happens when you type MORE than the buffer holds
 *     (a classic stack buffer overflow).
 *
 * Nothing here is a real exploit: the "canary" and "return address" values
 * are plain local variables we print so students can watch them get
 * clobbered. It is purely a visualization aid.
 *
 * Build:   gcc -Wall -Wextra -O0 -o stack_visualizer stack_visualizer.c
 * Run:     ./stack_visualizer
 *
 * Note: -O0 and -fno-stack-protector keep the layout predictable. The
 * Makefile in this directory sets those for you.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>

/* Size of the vulnerable buffer, in bytes. Small on purpose so overflows
 * are easy to trigger and easy to draw. */
#define BUF_SIZE 16

/* How many raw bytes we let read() pull in. Bigger than BUF_SIZE so the
 * user can overflow on demand and see the neighbours get trampled. */
#define READ_LIMIT 128

/*
 * A struct laid out so that, on a typical downward-growing stack, the
 * fields sit in memory in this order (low address -> high address):
 *
 *     buffer[0..BUF_SIZE-1]   <- what read() writes into
 *     saved_canary            <- our stand-in for the stack canary
 *     saved_base_pointer      <- our stand-in for saved RBP
 *     saved_return_address    <- our stand-in for the saved return address
 *
 * Because struct members are stored in declaration order, writing past the
 * end of `buffer` will spill into `saved_canary`, then the others. That is
 * exactly the phenomenon we want to visualize.
 */
typedef struct {
    unsigned char buffer[BUF_SIZE];
    uint64_t      saved_canary;
    uint64_t      saved_base_pointer;
    uint64_t      saved_return_address;
} stack_frame_t;

/* Return a printable representation of one byte for the ASCII column.
 * Printable characters are shown as-is; everything else as '.'. */
static char printable(unsigned char c)
{
    return isprint(c) ? (char)c : '.';
}

/*
 * Draw the buffer one byte per row so every character shows its own real
 * address in memory. For each slot we print:
 *
 *     address           index   hex   char   status
 *
 * The address is the genuine run-time address of that byte (&buffer[i]),
 * so students can see the buffer occupy a contiguous, increasing range and
 * relate byte offsets to concrete addresses.
 */
static void draw_buffer(const stack_frame_t *f, size_t bytes_written)
{
    printf("\n  buffer[%d]  (%zu byte%s written by read)\n",
           BUF_SIZE, bytes_written, bytes_written == 1 ? "" : "s");
    printf("  each row is one byte; the address is its real location in memory\n\n");

    printf("      address        index   hex   char   status\n");
    printf("   ------------------ ----- ------ ---- -----------\n");

    for (int idx = 0; idx < BUF_SIZE; idx++) {
        const unsigned char *addr = &f->buffer[idx];
        unsigned char c = f->buffer[idx];

        const char *status;
        char hexcol[7];
        char shown;

        if ((size_t)idx < bytes_written) {
            snprintf(hexcol, sizeof(hexcol), " 0x%02x ", c);
            shown = printable(c);
            status = "typed";
        } else if ((size_t)idx == bytes_written) {
            snprintf(hexcol, sizeof(hexcol), " 0x00 ");
            shown = '0';
            status = "NUL end";
        } else {
            snprintf(hexcol, sizeof(hexcol), "  ..  ");
            shown = ' ';
            status = "untouched";
        }

        printf("   %p  [%2d]  %s   %c   %s\n",
               (const void *)addr, idx, hexcol, shown, status);
    }
}

/* Print one of the "important" 64-bit slots that live above the buffer,
 * flagging it when it no longer holds its original sentinel value. The
 * `addr` is the slot's real run-time address, so it lines up numerically
 * with the per-byte addresses printed for the buffer below. */
static void draw_slot(const char *name, const void *addr,
                      uint64_t value, uint64_t original)
{
    const char *tag = (value == original) ? "intact" : ">>> OVERWRITTEN <<<";
    printf("  %p | %-22s | 0x%016" PRIx64 " |  %s\n",
           addr, name, value, tag);
}

/*
 * Draw the whole stack frame from high addresses (top of the picture) down
 * to the buffer (bottom), which mirrors how the stack is usually drawn in
 * lectures: the return address sits "above" the buffer that overflows up
 * into it.
 */
static void draw_stack(const stack_frame_t *f,
                       size_t bytes_written,
                       uint64_t canary0,
                       uint64_t rbp0,
                       uint64_t ret0)
{
    printf("\n");
    printf("  ============================================================\n");
    printf("   STACK STATE AFTER read()   (higher addresses on top)\n");
    printf("  ============================================================\n\n");

    printf("            higher addresses\n");
    printf("                  |\n");
    printf("                  v  (stack grows downward as calls are made)\n\n");

    printf("     address            slot                    value\n");
    draw_slot("saved return address", &f->saved_return_address, f->saved_return_address, ret0);
    draw_slot("saved base pointer",   &f->saved_base_pointer,   f->saved_base_pointer,   rbp0);
    draw_slot("stack canary",         &f->saved_canary,         f->saved_canary,         canary0);

    /* the buffer itself */
    printf("  |  char buffer[%2d]  (read() writes here, growing this way >>)\n",
           BUF_SIZE);
    draw_buffer(f, bytes_written);

    printf("\n");
    printf("                  ^\n");
    printf("                  |\n");
    printf("            lower addresses\n");

    /* Verdict */
    printf("\n  ------------------------------------------------------------\n");
    if (bytes_written < BUF_SIZE) {
        printf("  OK: %zu byte%s fit inside buffer[%d]. Nothing above was touched.\n",
               bytes_written, bytes_written == 1 ? "" : "s", BUF_SIZE);
    } else if (bytes_written == BUF_SIZE) {
        printf("  WARNING: buffer is exactly full (%d bytes). The NUL terminator\n",
               BUF_SIZE);
        printf("  has already spilled past buffer[%d] into the next slot.\n", BUF_SIZE - 1);
    } else {
        size_t overflow = bytes_written - BUF_SIZE;
        printf("  OVERFLOW: you wrote %zu byte%s past the end of buffer[%d]!\n",
               overflow, overflow == 1 ? "" : "s", BUF_SIZE);
        printf("  Those bytes have clobbered the canary / saved RBP / return\n");
        printf("  address shown above. In a real program this is how control\n");
        printf("  flow gets hijacked.\n");
    }
    printf("  ------------------------------------------------------------\n\n");
}

/*
 * The vulnerable read. We deliberately read up to READ_LIMIT bytes into a
 * BUF_SIZE buffer so students can choose to overflow it. We use fgets on a
 * separate raw buffer and then memcpy into the frame so we can control
 * exactly how many bytes land, mimicking an unbounded read() into the frame.
 */
static void do_read(stack_frame_t *f, size_t *bytes_written)
{
    char raw[READ_LIMIT + 1];

    printf("Type a string and press Enter (buffer holds %d bytes): ", BUF_SIZE);
    fflush(stdout);

    if (fgets(raw, sizeof(raw), stdin) == NULL) {
        printf("\n(no input read)\n");
        *bytes_written = 0;
        return;
    }

    /* Strip the trailing newline that fgets keeps. */
    size_t len = strlen(raw);
    if (len > 0 && raw[len - 1] == '\n') {
        raw[len - 1] = '\0';
        len--;
    }

    /* Copy every byte the user typed into the frame, starting at buffer[0].
     * This intentionally does NOT stop at BUF_SIZE: that is the whole point. */
    unsigned char *dst = f->buffer;
    for (size_t i = 0; i < len; i++)
        dst[i] = (unsigned char)raw[i];

    /* Write the NUL terminator too, just like a real string read would. */
    dst[len] = '\0';

    *bytes_written = len;
}

int main(void)
{
    stack_frame_t frame;

    /* Seed the "important" slots with recognizable sentinel values so it is
     * obvious when an overflow overwrites them. */
    const uint64_t CANARY0 = 0xDEADBEEFCAFEBABEULL;
    const uint64_t RBP0    = 0x00007FFFFFFFE000ULL;
    const uint64_t RET0    = 0x0000555555555189ULL;  /* pretend "return to main+..." */

    memset(frame.buffer, 0, sizeof(frame.buffer));
    frame.saved_canary        = CANARY0;
    frame.saved_base_pointer  = RBP0;
    frame.saved_return_address = RET0;

    printf("==============================================\n");
    printf(" Stack Visualizer  -  SI course teaching tool \n");
    printf("==============================================\n");
    printf("Buffer size: %d bytes. Read limit: %d bytes.\n", BUF_SIZE, READ_LIMIT);
    printf("Tip: type more than %d characters to trigger an overflow.\n\n", BUF_SIZE);

    size_t bytes_written = 0;
    do_read(&frame, &bytes_written);

    draw_stack(&frame, bytes_written, CANARY0, RBP0, RET0);

    return 0;
}

#include <uio.h>
#include <systm.h>
#include <stdc.h>
#include <vm_map.h>

static int copyin_vmspace(vm_map_t *vm, const void *restrict udaddr,
                          void *restrict kaddr, size_t len) {
  if (vm == get_kernel_vm_map()) {
    memcpy(kaddr, udaddr, len);
    return 0;
  }

  if (vm == get_user_vm_map()) {
    return copyin(udaddr, kaddr, len);
  }

  panic("copyin on non-active vm maps is not supported");
}

static int copyout_vmspace(vm_map_t *vm, const void *restrict kaddr,
                           void *restrict udaddr, size_t len) {
  if (vm == get_kernel_vm_map()) {
    memcpy(udaddr, kaddr, len);
    return 0;
  }

  if (vm == get_user_vm_map()) {
    return copyout(kaddr, udaddr, len);
  }

  panic("copyout on non-active vm maps is not supported");
}

/* Heavily inspired by NetBSD's uiomove */
/* This function modifies uio to reflect on the progress. */
int uiomove(void *buf, size_t n, uio_t *uio) {
  char *cbuf = buf + uio->uio_offset;
  int error = 0;

  assert(uio->uio_op == UIO_READ || uio->uio_op == UIO_WRITE);

  while (n > 0 && uio->uio_resid > 0) {
    /* Take the first io vector */
    iovec_t *iov = uio->uio_iov;
    size_t cnt = iov->iov_len;

    if (cnt == 0) {
      /* If no data left to move in this vector, proceed to the next io vector,
         or finish moving data if this was the last vector. */
      if (uio->uio_iovcnt == 0)
        break;
      uio->uio_iov++;
      uio->uio_iovcnt--;
      continue;
    }
    if (cnt > n)
      cnt = n;
    /* Perform copyout/copyin. */
    if (uio->uio_op == UIO_READ)
      error = copyout_vmspace(uio->uio_vmspace, cbuf, iov->iov_base, cnt);
    else
      error = copyin_vmspace(uio->uio_vmspace, iov->iov_base, cbuf, cnt);
    /* Exit immediately if there was a problem with moving data */
    if (error)
      break;

    /* Store progress on current io vector */
    iov->iov_base = (char *)iov->iov_base + cnt;
    iov->iov_len -= cnt;
    uio->uio_resid -= cnt;
    cbuf += cnt;
    n -= cnt;
  }

  return error;
}

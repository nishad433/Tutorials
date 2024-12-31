#include "qemu/osdep.h"
#include "hw/pci/pci_device.h"   // TYPE_PCI_DEVICE
#include "qemu/log.h"  // Make sure to include the logging header

#define TYPE_PCI_NCC_DEVICE "ncc-pci"

#define PCI_NCC_DEV_ID 0x4D4E

typedef struct NccState NccState;
DECLARE_INSTANCE_CHECKER(NccState, NCC,
                         TYPE_PCI_NCC_DEVICE)

struct NccState {
};

static void pci_ncc_realize(PCIDevice *pdev, Error **errp)
{
	qemu_log_mask(LOG_GUEST_ERROR, "NCC[%s:%d]\n",__func__,__LINE__);
}

static void pci_ncc_uninit(PCIDevice *pdev)
{
	qemu_log_mask(LOG_GUEST_ERROR, "NCC[%s:%d]\n",__func__,__LINE__);
}

static void ncc_instance_init(Object *obj)
{
	qemu_log_mask(LOG_GUEST_ERROR, "NCC[%s:%d]\n",__func__,__LINE__);
}

static void ncc_class_init(ObjectClass *class, void *data)
{
    qemu_log_mask(LOG_GUEST_ERROR, "NCC[%s:%d]\n",__func__,__LINE__);
    DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = pci_ncc_realize;
    k->exit = pci_ncc_uninit;
    k->vendor_id = PCI_VENDOR_ID_QEMU;
    k->device_id = PCI_NCC_DEV_ID;
    k->revision = 0x10;
    k->class_id = PCI_CLASS_OTHERS;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static void pci_ncc_register_types(void) {
	qemu_log_mask(LOG_GUEST_ERROR, "NCC[%s:%d]\n",__func__,__LINE__);
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };
    static const TypeInfo ncc_info = {
        .name          = TYPE_PCI_NCC_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(NccState),
        .instance_init = ncc_instance_init,
        .class_init    = ncc_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&ncc_info);
}

type_init(pci_ncc_register_types)

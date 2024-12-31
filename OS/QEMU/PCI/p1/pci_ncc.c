#include "qemu/osdep.h"
#include "hw/pci/pci_device.h"  // Defines the PCI device types and classes
#include "qemu/log.h"  // For logging various events

// Define the type name for our custom PCI device
#define TYPE_PCI_NCC_DEVICE "ncc-pci"

// Define the custom device's PCI vendor and device ID
#define PCI_NCC_DEV_ID 0x4D4E

// Declare the structure that will represent the device state
typedef struct NccState NccState;

// Declare an instance checker for the NccState object
DECLARE_INSTANCE_CHECKER(NccState, NCC, TYPE_PCI_NCC_DEVICE)

// Device state structure definition (empty for now, but can be extended)
struct NccState {
    // You can add state variables here as needed for the device
};

// Realize function: This is called when the device is created and initialized
static void pci_ncc_realize(PCIDevice *pdev, Error **errp)
{
    // Log the realization process of the NCC device
    qemu_log_mask(LOG_GUEST_ERROR, "NCC Realize called at %s:%d\n", __func__, __LINE__);
    // Additional device initialization code can go here
}

// Uninitialize function: This is called when the device is being removed or the VM shuts down
static void pci_ncc_uninit(PCIDevice *pdev)
{
    // Log the uninitialization process of the NCC device
    qemu_log_mask(LOG_GUEST_ERROR, "NCC Uninit called at %s:%d\n", __func__, __LINE__);
    // Any cleanup or resource deallocation should be done here
}

// Instance initialization: This function initializes a new instance of the NCC device
static void ncc_instance_init(Object *obj)
{
    // Log instance initialization
    qemu_log_mask(LOG_GUEST_ERROR, "NCC Instance init called at %s:%d\n", __func__, __LINE__);
    // Additional setup for the instance can go here
}

// Class initialization: This function is called when the class for the device is initialized
static void ncc_class_init(ObjectClass *class, void *data)
{
    // Log class initialization
    qemu_log_mask(LOG_GUEST_ERROR, "NCC Class init called at %s:%d\n", __func__, __LINE__);

    // Cast the class to appropriate device and PCI device classes
    DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    // Set up the PCI device class methods and properties
    k->realize = pci_ncc_realize;  // Link to the realize function
    k->exit = pci_ncc_uninit;      // Link to the uninitialize function
    k->vendor_id = PCI_VENDOR_ID_QEMU;  // Set vendor ID
    k->device_id = PCI_NCC_DEV_ID;  // Set device ID (unique identifier for our device)
    k->revision = 0x10;  // Set the revision number (optional)
    k->class_id = PCI_CLASS_OTHERS;  // Set the PCI class ID (generic class)

    // Assign device category
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);  // Set the category to "miscellaneous"
}

// Register the PCI device type in QEMU's type system
static void pci_ncc_register_types(void)
{
    // Log type registration
    qemu_log_mask(LOG_GUEST_ERROR, "NCC Register types called at %s:%d\n", __func__, __LINE__);

    // Define the interfaces that the device will implement
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },  // Standard PCI device interface
        { },  // End of interfaces list
    };

    // Define the TypeInfo structure for the NCC PCI device
    static const TypeInfo ncc_info = {
        .name          = TYPE_PCI_NCC_DEVICE,  // Type name for this device
        .parent        = TYPE_PCI_DEVICE,      // Parent class (PCI device)
        .instance_size = sizeof(NccState),    // Size of the device state structure
        .instance_init = ncc_instance_init,   // Instance initialization function
        .class_init    = ncc_class_init,      // Class initialization function
        .interfaces    = interfaces,          // List of interfaces supported by the device
    };

    // Register the device type with QEMU's type system
    type_register_static(&ncc_info);
}

// Initialize the PCI device type registration at runtime
type_init(pci_ncc_register_types)

package main

import (
	"bytes"
	"fmt"
	"github.com/google/gopacket"
	"github.com/google/gopacket/layers"
	"github.com/google/gopacket/pcap"
	"github.com/mistsys/mist_go_utils/net/ethernet"
	"github.com/mistsys/mist_go_utils/net/wifi"
	"log"
	"os"
)

type mistbssInfo struct {
	capability    uint16
	ssid          string
	siteInfo      string
	rssi          wifi.RSSI
	bssid         ethernet.MAC
	channel       wifi.Channel
	ie_channelset wifi.ChannelSet
}

func freq2Channel(chanFreq uint16) wifi.Channel {
	ch, ok := wifi.ChannelFromMHz(wifi.Frequency(chanFreq))
	if ok {
		return ch
	}
	return 0
}

func processBeacon(no int, packet gopacket.Packet) {
	radioTapLayer := packet.Layer(layers.LayerTypeRadioTap)
	if radioTapLayer == nil {
		return
	}
	rt, _ := radioTapLayer.(*layers.RadioTap)
	channel := freq2Channel(uint16(rt.ChannelFrequency))
	if channel == 0 {
		return
	}

	d11Layer := packet.Layer(layers.LayerTypeDot11)
	if d11Layer == nil {
	}
	d11, _ := d11Layer.(*layers.Dot11)
	if (d11.Type.MainType() != layers.Dot11TypeMgmt) || (d11.Type.MainType() == layers.Dot11TypeMgmt && d11.Type != layers.Dot11TypeMgmtBeacon) {
		return
	}
	bssid := ethernet.MakeMACFromHardwareAddr(d11.Address3)
	beacon, ok := packet.Layer(layers.LayerTypeDot11MgmtBeacon).(*layers.Dot11MgmtBeacon)
	if ok {
		beaconpack := gopacket.NewPacket(beacon.LayerContents(), layers.LayerTypeDot11MgmtBeacon, gopacket.Default)
		mistOUI := []byte{0x5c, 0x5b, 0x35, 0x02}

		var beaconRecord mistbssInfo
		var ie_ht_bw, ie_vht_bw wifi.Width
		var ht_ie_present, vht_ie_present, vht_160_supported bool

		beaconRecord.capability = uint16(beacon.Flags)
		beaconRecord.rssi = wifi.RSSI(rt.DBMAntennaSignal)
		beaconRecord.channel = channel
		band := channel.Band()
		beaconRecord.bssid = bssid
		beaconRecord.ie_channelset.Width = wifi.Width20MHz

		for _, l := range beaconpack.Layers() {
			infoElem, ok := l.(*layers.Dot11InformationElement)
			if !ok {
				continue
			}
			if band == wifi.Band6GHz {
				if infoElem.ID == 0xff {
					fmt.Printf("NCC inforElem:%+v\n",infoElem.Length)
					if infoElem.Info[0] == 0x24 {
						he_ops_param := (uint32(infoElem.Info[3]) << 16) | (uint32(infoElem.Info[2]) << 8) | uint32(infoElem.Info[1])
						if he_ops_param&0x020000 != 0 {
							ops_info_6ghz_offset := 7
							if he_ops_param&0x008000 != 0 {
								ops_info_6ghz_offset += 1
							}
							ieChannelNo := wifi.ChannelNumber(infoElem.Info[ops_info_6ghz_offset])
							beaconRecord.ie_channelset.Primary = wifi.MakeChannel(ieChannelNo, band)
							chan_ctrl := uint8(infoElem.Info[ops_info_6ghz_offset+1]) & 0x3
							switch chan_ctrl {
							case 0:
								beaconRecord.ie_channelset.Width = wifi.Width20MHz
							case 1:
								beaconRecord.ie_channelset.Width = wifi.Width40MHz
							case 2:
								beaconRecord.ie_channelset.Width = wifi.Width80MHz
							case 3:
								beaconRecord.ie_channelset.Width = wifi.Width160MHz
							}
						}
					}
				}
			} else if band == wifi.Band5GHz {
				if infoElem.ID == layers.Dot11InformationElementIDVHTCapabilities {
					vht_ie_present = true
					supportedChannelWidthSet := (infoElem.Info[0] & 0xc) >> 2
					if supportedChannelWidthSet != 0 {
						vht_160_supported = true
					}
				} else if infoElem.ID == layers.Dot11InformationElementIDVHTOperation {
					vht_ie_present = true
					if infoElem.Info[0] == 1 {
						ie_vht_bw = wifi.Width80MHz
					} else {
						ie_vht_bw = wifi.Width20MHz
					}
				} else if infoElem.ID == layers.Dot11InformationElementIDHTInfo {
					ht_ie_present = true
					ieChannelNo := wifi.ChannelNumber(infoElem.Info[0])
					beaconRecord.ie_channelset.Primary = wifi.MakeChannel(ieChannelNo, band)
					if infoElem.Info[1]&0x3 == 0 {
						ie_ht_bw = wifi.Width20MHz
					} else {
						ie_ht_bw = wifi.Width40MHz
					}
				}
			} else if band == wifi.Band24GHz {
				if infoElem.ID == layers.Dot11InformationElementIDHTInfo {
					ieChannelNo := wifi.ChannelNumber(infoElem.Info[0])
					beaconRecord.ie_channelset.Primary = wifi.MakeChannel(ieChannelNo, band)
					sco := infoElem.Info[1] & 0x3
					switch sco {
					case 0:
						beaconRecord.ie_channelset.Width = wifi.Width20MHz
					case 1:
						secondaryChNo := beaconRecord.ie_channelset.Primary.Number() + 5
						if secondaryChNo > 0 && secondaryChNo < 14 {
							beaconRecord.ie_channelset.Secondary = wifi.MakeChannel(secondaryChNo, band)
						}
						beaconRecord.ie_channelset.Width = wifi.Width40MHz
					case 3:
						secondaryChNo := beaconRecord.ie_channelset.Primary.Number() - 5
						if secondaryChNo > 0 && secondaryChNo < 14 {
							beaconRecord.ie_channelset.Secondary = wifi.MakeChannel(secondaryChNo, band)
						}
						beaconRecord.ie_channelset.Width = wifi.Width40MHz
					}
				}
			}

			if infoElem.ID == layers.Dot11InformationElementIDSSID {
				beaconRecord.ssid = string(infoElem.Info)
			}
			if infoElem.ID == layers.Dot11InformationElementIDVendor && bytes.Compare(infoElem.OUI, mistOUI) == 0 {
				beaconRecord.siteInfo = string(infoElem.Info)
			}
		}
		if band == wifi.Band5GHz {
			if vht_ie_present && ie_vht_bw == wifi.Width80MHz {
				if vht_160_supported {
					beaconRecord.ie_channelset.Width = wifi.Width160MHz
				} else {
					beaconRecord.ie_channelset.Width = wifi.Width80MHz
				}
			} else if ht_ie_present {
				beaconRecord.ie_channelset.Width = ie_ht_bw
			}
		}
		if beaconRecord.ie_channelset.Primary == beaconRecord.channel {
			fmt.Printf("bssInfo[%d]=%+v\n\n", no, beaconRecord)

		} else {
			fmt.Printf("mismatch bssInfo[%d]=%+v\n\n", no, beaconRecord)
		}

	}

}

func main() {
	filename := os.Args[1]
	fmt.Printf("filename %s\n", filename)
	// Open file instead of device
	handle, err := pcap.OpenOffline(filename)
	if err != nil {
		log.Fatal(err)
	}
	defer handle.Close()

	// Loop through packets in file
	packetSource := gopacket.NewPacketSource(handle, handle.LinkType())
	i := 0
	for packet := range packetSource.Packets() {
		i = i + 1
		processBeacon(i, packet)
	}
}

package main

import (
	"encoding/json"
	"fmt"
	"github.com/mistsys/mist_go_utils/net/ethernet"
	"github.com/mistsys/mist_go_utils/net/wifi"
	"io"
	"io/ioutil"
	"os"
	"sort"
	"strings"
)

var filename1, filename2 string

type commonQ struct {
	SSID          string       `json:"SSID"`
	BSSID         ethernet.MAC `json:"BSSID"`
	RSSI_1        wifi.RSSI    `json:"RSSI"`
	RSSI_2        wifi.RSSI    `json:"RSSI"`
	ListenChannel wifi.Channel `json:"ListenChannel"` // listening channel (RX channel)
}
type ScanResults struct {
	SSID           string          `json:"SSID"`
	BSSID          ethernet.MAC    `json:"BSSID"`
	RSSI           wifi.RSSI       `json:"RSSI"`
	Channel        wifi.ChannelSet `json:"Channel"`        // note: name of field is historical. don't touch.
	PotentialRogue bool            `json:"PotentialRogue"` // potential rogue as detected by the AP
	IsSpoof        bool            `json:"IsSpoof"`        // is spoofing the reporting AP
	ListenChannel  wifi.Channel    `json:"ListenChannel"`  // listening channel (RX channel)
}

type WifiScanResults struct {
	ID        ethernet.MAC  `json:"ID"`
	ReportAll bool          `json:"ReportAll"`
	Scan24GHz []ScanResults `json:"Scan24GHz"`
	Scan5GHz  []ScanResults `json:"Scan5GHz"`
	Scan6GHz  []ScanResults `json:"Scan6GHz"`
}

func display(s []ScanResults, band wifi.Band) {
	bssidCount := make(map[wifi.Channel]int)
	sort.SliceStable(s, func(i, j int) bool {
		return s[i].RSSI > s[j].RSSI
	})

	if len(s) > 0 {
		fmt.Printf("######################################################################################\n\n")
		fmt.Printf("%v ScanResults: BSSID Count:=%d\n", band, len(s))
		for i := range s {
			bssidCount[s[i].ListenChannel] += 1
			fmt.Printf("%-3d]Band=%v, BSSID=%v, RSSI=%v, Channel=%-3v, SSID=\"%s\"\n", i+1, band, s[i].BSSID, s[i].RSSI, s[i].ListenChannel, s[i].SSID)
			//fmt.Printf("%-3d]Band=%v, BSSID=%v, RSSI=%v, Channel=%v,\n", i+1, band, s[i].BSSID, s[i].RSSI, s[i].ListenChannel)
		}
		if len(bssidCount) > 0 {
			keys := make([]wifi.Channel, 0, len(bssidCount))
			fmt.Printf("\n\nBSSID ChannelMap for Band %v Total_BSSIDcnt=%v\n", band, len(s))
			for key, _ := range bssidCount {
				keys = append(keys, key)
			}
			sort.SliceStable(keys, func(i, j int) bool {
				return keys[i] < keys[j]
			})

			for i, key := range keys {
				fmt.Printf("%-2d]Channel=%-3v, BSSIDcnt=%v\n", i, key, bssidCount[key])
			}
		}
		fmt.Printf("\n\n######################################################################################\n")
	} else {
		fmt.Printf("######################################################################################\n\n")
		fmt.Printf("%v ScanResults: BSSID Count:=%d\n", band, len(s))
		fmt.Printf("\n######################################################################################\n")
	}
}

func getWifiScanResults(filename string) ([]WifiScanResults, error) {
	f := []WifiScanResults{}
	n_array := []WifiScanResults{}
	//	return "", errors.New("empty name")
	fmt.Printf("Reading File: %s\n", filename)
	jsonFile, err := os.Open(filename)
	// if we os.Open returns an error then handle it
	if err != nil {
		return f, err
	}

	// defer the closing of our jsonFile so that we can parse it later on
	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	lines := strings.Split(string(byteValue), "\n")

	var outfile string
	for _, line := range lines {
		line = strings.TrimSpace(line)
		if len(line) > 0 && line[0] != '#' {
			outfile += line + "\n"
		}
	}
	jsonParser := json.NewDecoder(strings.NewReader(outfile))

	for {
		n := WifiScanResults{}

		err := jsonParser.Decode(&n)
		if err == io.EOF {
			// all done
			break
		}
		if err != nil {
			fmt.Printf("Error\n")
		}
		n_array = append(n_array, n)
	}

	for i := range n_array {
		if n_array[i].ReportAll {
			f = append(f, n_array[i])
		}
	}
	return f, nil
}

func displayF(f WifiScanResults, i int) {

	fmt.Printf("\nFull Report[%d] for AP MAC:%v\n", i, f.ID)
	display(f.Scan24GHz, wifi.Band24GHz)
	display(f.Scan5GHz, wifi.Band5GHz)
	display(f.Scan6GHz, wifi.Band6GHz)
	fmt.Printf("\n\n\n")

}

type histT struct {
	bins     int
	binWidth int
	hist     []int
}

func (h *histT) init(bins int) {
	h.bins = bins
	h.binWidth = 30 / ((bins-1)/2 - 1)
	h.hist = make([]int, bins)
}

func (h *histT) add(val wifi.RSSI) int {
	idx := 0
	i := 0
	if val == wifi.RSSI(0) {
		idx = h.bins / 2
	} else if val < wifi.RSSI(0) {
		idx = 0
		val = val * (-1)
	} else if val > wifi.RSSI(0) {
		idx = h.bins / 2
	}
	if val != 0 {
		for i = 0; i < (h.bins / 2); i++ {
			if val <= 0 {
				break
			} else {
				val -= wifi.RSSI(h.binWidth)
			}
		}
		if idx == 0 {
			idx = (h.bins/2 - i)
		} else {
			idx += i
		}
	}
	h.hist[idx]++
	return idx

}
func (h *histT) printHist(id1 ethernet.MAC, id2 ethernet.MAC, band wifi.Band) {

	fmt.Printf("\n\nRSSI Histogram for AP1:%v AP2:%v Band %v\n\n", id1, id2, band)
	for i := range h.hist {
		if i == h.bins/2 {
			fmt.Printf("%6s0dB%6s = %d\n", "", "", h.hist[i])
		} else if i > h.bins/2 {
			j := i - h.bins/2
			if i == h.bins-1 {
				s := (j - 1) * h.binWidth
				fmt.Printf("[%6ddB+%4s] = %d\n", s, "", h.hist[i])
			} else {
				s := 1 + (j-1)*h.binWidth
				e := (j) * h.binWidth
				fmt.Printf("[%3ddB : %3ddB] = %d\n", s, e, h.hist[i])
			}
		} else if i < h.bins/2 {
			j := (h.bins / 2) - i
			if i == 0 {
				s := 0 - (j-1)*h.binWidth
				fmt.Printf("[%6ddB+%4s] = %d\n", s, "", h.hist[i])
			} else {
				s := 0 - (1 + (j-1)*h.binWidth)
				e := 0 - ((j) * h.binWidth)
				fmt.Printf("[%3ddB : %3ddB] = %d\n", s, e, h.hist[i])
			}
		}
	}

}

func compareScan(f1 []ScanResults, id1 ethernet.MAC, f2 []ScanResults, id2 ethernet.MAC, band wifi.Band, i int) {

	var hT histT
	h := &hT
	h.init(11)

	fmt.Printf("\n\nComparing Band %v for AP MAC1:%v(count %v) MAC2:%v(count %v) from FullReport[%d]\n", band, id1, len(f1), id2, len(f2), i)
	sort.SliceStable(f1, func(i, j int) bool {
		return f1[i].RSSI > f1[j].RSSI
	})
	sort.SliceStable(f2, func(i, j int) bool {
		return f2[i].RSSI > f2[j].RSSI
	})
	common := []commonQ{}
	f1Q := []commonQ{}
	f2Q := []commonQ{}

	i1 := len(f1)
	i2 := len(f2)
	found := false

	for i := 0; i < i1; i++ {
		found = false
		comm := commonQ{}
		comm.BSSID = f1[i].BSSID
		comm.SSID = f1[i].SSID
		comm.RSSI_1 = f1[i].RSSI
		comm.ListenChannel = f1[i].ListenChannel
		for j := 0; j < i2; j++ {
			comm.RSSI_2 = f2[j].RSSI
			if f1[i].BSSID == f2[j].BSSID {
				common = append(common, comm)
				found = true
			}
		}
		if !found {
			f1Q = append(f1Q, comm)
		}
	}
	for i := 0; i < i2; i++ {
		found = false
		comm := commonQ{}
		comm.BSSID = f2[i].BSSID
		comm.SSID = f2[i].SSID
		comm.RSSI_2 = f2[i].RSSI
		comm.ListenChannel = f2[i].ListenChannel
		for j := 0; j < i1; j++ {
			comm.RSSI_1 = f1[j].RSSI
			if f2[i].BSSID == f1[j].BSSID {
				found = true
			}
		}
		if !found {
			f2Q = append(f2Q, comm)
		}
	}
	fmt.Printf("\n\nCommonCount:%v, Only on AP %v(%v):%v, Only on AP %v(%v):%v\n\n", len(common), id1, filename1, len(f1Q), id2, filename2, len(f2Q))

	if len(common) > 0 {
		fmt.Printf("\n\nCommon BSSID count=%v Band %v\n", len(common), band)
		for i := range common {
			c := common[i]
			diff := c.RSSI_1 - c.RSSI_2
			_ = h.add(diff)

			fmt.Printf("%-2d]BSSID=%v, RSSI(on %v)=%v, RSSI(on %v)=%v, RSSI_diff=%v, Channel=%v SSID=\"%s\" (%v)\n", i, c.BSSID, id1, c.RSSI_1, id2, c.RSSI_2, diff, c.ListenChannel, c.SSID, band)
			//	fmt.Printf("%-2d]BSSID=%v, RSSI(on %v)=%v, RSSI(on %v)=%v RSSI_diff=%v, Channel=%v, (%v)\n", i, c.BSSID, id1, c.RSSI_1, id2, c.RSSI_2,diff, c.ListenChannel, band)
		}

		h.printHist(id1, id2, band)
	}
	if len(f1Q) > 0 {
		fmt.Printf("\n\nOnly on AP %v (%v) BSSID count=%v Band %v\n", id1, filename1, len(f1Q), band)
		for i := range f1Q {
			c := f1Q[i]
			fmt.Printf("%-2d]BSSID=%v, RSSI=%v, Channel=%v SSID=\"%s\" (%v)\n", i, c.BSSID, c.RSSI_1, c.ListenChannel, c.SSID, band)
			//fmt.Printf("%-2d]BSSID=%v, RSSI=%v, Channel=%v, (%v)\n", i, c.BSSID, c.RSSI_1, c.ListenChannel, band)
		}
	}
	if len(f2Q) > 0 {
		fmt.Printf("\n\nOnly on AP %v (%v) BSSID count=%v Band %v\n", id2, filename2, len(f2Q), band)
		for i := range f2Q {
			c := f2Q[i]
			fmt.Printf("%-2d]BSSID=%v, RSSI=%v, Channel=%v SSID=\"%s\" (%v)\n", i, c.BSSID, c.RSSI_2, c.ListenChannel, c.SSID, band)
			//fmt.Printf("%-2d]BSSID=%v, RSSI=%v, Channel=%v, (%v)\n", i, c.BSSID, c.RSSI_2, c.ListenChannel, band)
		}
	}
}

func compareFs(f1 WifiScanResults, f2 WifiScanResults, i int) {
	compareScan(f1.Scan24GHz, f1.ID, f2.Scan24GHz, f2.ID, wifi.Band24GHz, i)
	compareScan(f1.Scan5GHz, f1.ID, f2.Scan5GHz, f2.ID, wifi.Band5GHz, i)
	compareScan(f1.Scan6GHz, f1.ID, f2.Scan6GHz, f2.ID, wifi.Band6GHz, i)
}

func main() {

	// Open our jsonFile
	filename1 = os.Args[1]
	filename2 = os.Args[2]

	f1_a, err1 := getWifiScanResults(filename1)
	f2_a, err2 := getWifiScanResults(filename2)

	n := 0
	if len(f1_a) > len(f2_a) {
		n = len(f2_a)
	} else {
		n = len(f1_a)
	}

	for i := 0; i < n; i++ {
		fmt.Printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n\n")
		f1 := f1_a[i]
		f2 := f2_a[i]
		if err1 == nil {
			displayF(f1, i)
		}
		if err2 == nil {
			displayF(f2, i)
		}
		if err1 == nil && err2 == nil {
			fmt.Printf("\n\n\n##############################################################################################################################\n")
			fmt.Printf("\n##############################################################################################################################\n")
			fmt.Printf("\n##############################################################################################################################\n")
			fmt.Printf("\n##############################################################################################################################\n")
			fmt.Printf("\n\n\nComparison Results\n")
			compareFs(f1, f2, i)
		}
		fmt.Printf("\n\n\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
		fmt.Printf("\n|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n")
	}

}

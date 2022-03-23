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
			fmt.Printf("%-3d]Band=%v, BSSID=%v, RSSI=%v, Channel=%-3v, SSID=\"%v\"\n", i+1, band, s[i].BSSID, s[i].RSSI, s[i].ListenChannel, s[i].SSID)
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

func main() {

	n_array := []WifiScanResults{}
	// Open our jsonFile
	filename := os.Args[1]
	fmt.Printf("Reading File: %s\n", filename)
	jsonFile, err := os.Open(filename)
	// if we os.Open returns an error then handle it
	if err != nil {
		fmt.Println(err)
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

	j := 0
	for i := range n_array {
		if n_array[i].ReportAll {
			j += 1
			fmt.Printf("\n\nFull Report %d\n", j)
			display(n_array[i].Scan24GHz, wifi.Band24GHz)
			display(n_array[i].Scan5GHz, wifi.Band5GHz)
			display(n_array[i].Scan6GHz, wifi.Band6GHz)
		}
	}

}

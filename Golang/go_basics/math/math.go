package math

func Add(a, b int) (int, bool) {
	return a + b, true
}

func Sub(a, b int) (int, bool) {
	return a - b, true
}

func Mul(a, b int) (int, bool) {
	return a * b, true
}

func Div(a, b int) (int, bool) {
	if b == 0 {
		return 0, false
	} else {
		return a / b, true
	}
}

// This fn is not exposed to outside world
func isPrimeNo(No int) bool {
	for i := 2; i < No/2; i++ {
		if No%i == 0 {
			return false
		}
	}
	return true
}

func GetPrimeNos(N int) []int {

	var ret []int
	for i := 1; i <= N; i++ {
		if isPrimeNo(i) {
			ret = append(ret, i)
		}
	}
	return ret
}

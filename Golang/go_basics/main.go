package main

import (
	"fmt"
	"github.com/nishad433/go_basics/math"
	"github.com/nishad433/go_basics/ncc_structs"
)

// global variables
// Uppercase'ing First letter of VariableName, makes the variable accessible from other files

var Author string = "Nishad"
var Description = "Go Basics"

func greeting(author, desc string) string {
	return desc + " by " + author + "\n"
}

func math_ops(a, b int) {
	c, ok := math.Div(a, b)
	if ok {
		fmt.Printf("%d+%d=%d\n", a, b, c)
	} else {
		fmt.Printf("div by 0\n")
	}

	c, _ = math.Add(a, b)
	fmt.Printf("%d+%d=%d\n", a, b, c)

	c, _ = math.Sub(a, b)
	fmt.Printf("%d-%d=%d\n", a, b, c)

	c, _ = math.Mul(a, b)
	fmt.Printf("%d*%d=%d\n", a, b, c)

}

func getArea(s ncc_structs.Shape) float64 {
	return s.Area()
}

func shapes() {
	c := ncc_structs.Circle{Radius: 5}
	r := ncc_structs.Rectangle{4, 7}

	fmt.Printf("Radius=%f Area=%f Area=%f\n", c.Radius, c.Area(), getArea(c))
	fmt.Printf("Length=%f Breadth=%f Area=%f Area=%f\n", r.Length, r.Breadth, r.Area(), getArea(r))
}

func main() {
	//fmt.Printf("%s by %s\n",Description,Author);
	fmt.Printf(greeting(Author, Description))

	math_ops(15, 5)

	fmt.Printf("PrimeNos:%v\n", math.GetPrimeNos(30))

	shapes()

}

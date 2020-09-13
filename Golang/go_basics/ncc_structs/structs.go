package ncc_structs

const (
	PI float64 = 3.14
)

type Circle struct {
	Radius float64
}

func (c Circle) Area() float64 {
	return PI * c.Radius * c.Radius
}

type Rectangle struct {
	Length, Breadth float64
}

func (r Rectangle) Area() float64 {
	return r.Length * r.Breadth
}

type Shape interface {
	Area() float64
}

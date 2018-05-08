#include <stdio.h>
#include <stdlib.h>
#include "Semantic.h"

Attribute * Create_Attribute()
{
	return (Attribute *)calloc(1, sizeof(Attribute));
}

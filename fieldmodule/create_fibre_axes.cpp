
#include <gtest/gtest.h>

#include <zinc/zincconfigure.h>
#include <zinc/core.h>
#include <zinc/context.h>
#include <zinc/region.h>
#include <zinc/fieldmodule.h>
#include <zinc/field.h>
#include <zinc/fieldfibres.h>
#include <zinc/fieldconstant.h>

TEST(cmzn_field_module_create_fibre_axes, invalid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_field_module_id fm = cmzn_region_get_field_module(root_region);

	EXPECT_NE(static_cast<cmzn_field_module *>(0), fm);

	cmzn_field_id f0 = cmzn_field_module_create_fibre_axes(0, 0, 0);
	EXPECT_EQ(0, f0);

	cmzn_field_id f1 = cmzn_field_module_create_fibre_axes(fm, 0, 0);
	EXPECT_EQ(0, f1);

	double values[] = {3.0, 2.0, 1.0, 7.0};
	cmzn_field_id f2 = cmzn_field_module_create_constant(fm, 3, values);
	cmzn_field_id f3 = cmzn_field_module_create_constant(fm, 4, values);

	cmzn_field_id f4 = cmzn_field_module_create_fibre_axes(fm, f2, f3);
	EXPECT_EQ(0, f4);

	cmzn_field_destroy(&f0);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&f3);
	cmzn_field_destroy(&f4);
	cmzn_field_module_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}


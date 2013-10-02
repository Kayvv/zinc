
#include <gtest/gtest.h>

#include <zinc/zincconfigure.h>
#include <zinc/context.h>
#include <zinc/core.h>
#include <zinc/differentialoperator.h>
#include <zinc/element.h>
#include <zinc/field.h>
#include <zinc/fieldcache.h>
#include <zinc/fieldconstant.h>
#include <zinc/fieldderivatives.h>
#include <zinc/fieldmodule.h>
#include <zinc/fieldvectoroperators.h>
#include <zinc/node.h>
#include <zinc/region.h>
#include <zinc/status.h>

#include "test_resources.h"

#include "zinctestsetup.hpp"

TEST(cmzn_fieldmodule_create_field_derivative, invalid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	EXPECT_NE(static_cast<cmzn_fieldmodule *>(0), fm);

	cmzn_field_id f0 = cmzn_fieldmodule_create_field_derivative(0, 0, 0);
	EXPECT_EQ(0, f0);

	cmzn_field_id f1 = cmzn_fieldmodule_create_field_derivative(fm, 0, 2);
	EXPECT_EQ(0, f1);

	double values[] = {6.0, 1.0, 2.5};
	cmzn_field_id f2 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f3 = cmzn_fieldmodule_create_field_derivative(fm, f2, -1);
	EXPECT_EQ(0, f3);

	cmzn_field_destroy(&f0);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&f3);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_derivative, valid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	cmzn_region_read_file(root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));

	cmzn_field_id f1 = cmzn_fieldmodule_find_field_by_name(fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), f1);

	cmzn_field_id ft = cmzn_fieldmodule_find_field_by_name(fm, "xi");
	EXPECT_NE(static_cast<cmzn_field *>(0), ft);

	cmzn_field_id f2 = cmzn_fieldmodule_create_field_derivative(fm, f1, 1);
	EXPECT_NE(static_cast<cmzn_field *>(0), f2);

	cmzn_fieldcache_id fc = cmzn_fieldmodule_create_fieldcache(fm);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(fm, 3);
	EXPECT_NE(static_cast<cmzn_mesh *>(0), mesh);

	cmzn_element_id el = cmzn_mesh_find_element_by_identifier(mesh, 1);
	EXPECT_NE(static_cast<cmzn_element *>(0), el);

	double chart_coordinates[] = {0.6, 0.2, 0.45};
	int result = cmzn_fieldcache_set_mesh_location(fc, el, 3, chart_coordinates);
	EXPECT_EQ(CMZN_OK, result);

	double outvalues[3];
	result = cmzn_field_evaluate_real(f2, fc, 3, outvalues);
	EXPECT_EQ(CMZN_OK, result);
	EXPECT_EQ(1.0, outvalues[0]);
	EXPECT_EQ(0.0, outvalues[1]);
	EXPECT_EQ(0.0, outvalues[2]);

	cmzn_element_destroy(&el);
	cmzn_mesh_destroy(&mesh);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&ft);
	cmzn_fieldcache_destroy(&fc);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_curl, invalid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	EXPECT_NE(static_cast<cmzn_fieldmodule *>(0), fm);

	cmzn_field_id f0 = cmzn_fieldmodule_create_field_curl(0, 0, 0);
	EXPECT_EQ(0, f0);

	cmzn_field_id f1 = cmzn_fieldmodule_create_field_curl(fm, 0, 0);
	EXPECT_EQ(0, f1);

	double values[] = {6.0, 1.0, 2.5};
	cmzn_field_id f2 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f3 = cmzn_fieldmodule_create_field_curl(fm, f2, 0);
	EXPECT_EQ(0, f3);

	cmzn_field_destroy(&f0);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&f3);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_curl, valid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	cmzn_region_read_file(root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));

	cmzn_field_id f1 = cmzn_fieldmodule_find_field_by_name(fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), f1);

	double values[] = {2.0, 3.0, 5.0};
	cmzn_field_id c1 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f2 = cmzn_fieldmodule_create_field_curl(fm, c1, f1);
	EXPECT_NE(static_cast<cmzn_field *>(0), f2);

	cmzn_fieldcache_id fc = cmzn_fieldmodule_create_fieldcache(fm);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(fm, 3);
	EXPECT_NE(static_cast<cmzn_mesh *>(0), mesh);

	cmzn_element_id el = cmzn_mesh_find_element_by_identifier(mesh, 1);
	EXPECT_NE(static_cast<cmzn_element *>(0), el);

	double chart_coordinates[] = {0.6, 0.2, 0.45};
	int result = cmzn_fieldcache_set_mesh_location(fc, el, 3, chart_coordinates);
	EXPECT_EQ(CMZN_OK, result);

	double outvalues[3];
	result = cmzn_field_evaluate_real(f2, fc, 3, outvalues);
	EXPECT_EQ(CMZN_OK, result);
	EXPECT_EQ(0.0, outvalues[0]);
	EXPECT_EQ(0.0, outvalues[1]);
	EXPECT_EQ(0.0, outvalues[2]);

	cmzn_element_destroy(&el);
	cmzn_mesh_destroy(&mesh);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&c1);
	cmzn_field_destroy(&f2);
	cmzn_fieldcache_destroy(&fc);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_divergence, invalid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	EXPECT_NE(static_cast<cmzn_fieldmodule *>(0), fm);

	cmzn_field_id f0 = cmzn_fieldmodule_create_field_divergence(0, 0, 0);
	EXPECT_EQ(0, f0);

	cmzn_field_id f1 = cmzn_fieldmodule_create_field_divergence(fm, 0, 0);
	EXPECT_EQ(0, f1);

	double values[] = {6.0, 1.0, 2.5};
	cmzn_field_id f2 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f3 = cmzn_fieldmodule_create_field_divergence(fm, f2, 0);
	EXPECT_EQ(0, f3);

	cmzn_field_destroy(&f0);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&f3);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_divergence, valid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	cmzn_region_read_file(root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));

	cmzn_field_id f1 = cmzn_fieldmodule_find_field_by_name(fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), f1);

	double values[] = {2.0, 3.0, 5.0};
	cmzn_field_id c1 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f2 = cmzn_fieldmodule_create_field_divergence(fm, c1, f1);
	EXPECT_NE(static_cast<cmzn_field *>(0), f2);

	cmzn_fieldcache_id fc = cmzn_fieldmodule_create_fieldcache(fm);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(fm, 3);
	EXPECT_NE(static_cast<cmzn_mesh *>(0), mesh);

	cmzn_element_id el = cmzn_mesh_find_element_by_identifier(mesh, 1);
	EXPECT_NE(static_cast<cmzn_element *>(0), el);

	double chart_coordinates[] = {0.6, 0.2, 0.45};
	int result = cmzn_fieldcache_set_mesh_location(fc, el, 3, chart_coordinates);
	EXPECT_EQ(CMZN_OK, result);

	double outvalues[1];
	result = cmzn_field_evaluate_real(f2, fc, 1, outvalues);
	EXPECT_EQ(CMZN_OK, result);
	EXPECT_EQ(0.0, outvalues[0]);

	cmzn_element_destroy(&el);
	cmzn_mesh_destroy(&mesh);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&c1);
	cmzn_field_destroy(&f2);
	cmzn_fieldcache_destroy(&fc);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_divergence, grad_mag)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	cmzn_region_read_file(root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));

	cmzn_field_id f1 = cmzn_fieldmodule_find_field_by_name(fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), f1);

	cmzn_field_id c1 = cmzn_fieldmodule_create_field_magnitude(fm, f1);

	cmzn_field_id c2 = cmzn_fieldmodule_create_field_gradient(fm, c1, f1);

	cmzn_field_id f2 = cmzn_fieldmodule_create_field_divergence(fm, c2, f1);
	EXPECT_NE(static_cast<cmzn_field *>(0), f2);

	cmzn_fieldcache_id fc = cmzn_fieldmodule_create_fieldcache(fm);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(fm, 3);
	EXPECT_NE(static_cast<cmzn_mesh *>(0), mesh);

	cmzn_element_id el = cmzn_mesh_find_element_by_identifier(mesh, 1);
	EXPECT_NE(static_cast<cmzn_element *>(0), el);

	double chart_coordinates[] = {0.6, 0.2, 0.45};
	int result = cmzn_fieldcache_set_mesh_location(fc, el, 3, chart_coordinates);
	EXPECT_EQ(CMZN_OK, result);

	double outvalues[1];
	result = cmzn_field_evaluate_real(f2, fc, 1, outvalues);
	EXPECT_NE(CMZN_OK, result);
	//EXPECT_EQ(0.0, outvalues[0]);

	cmzn_element_destroy(&el);
	cmzn_mesh_destroy(&mesh);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&c1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&c2);
	cmzn_fieldcache_destroy(&fc);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_gradient, invalid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	EXPECT_NE(static_cast<cmzn_fieldmodule *>(0), fm);

	cmzn_field_id f0 = cmzn_fieldmodule_create_field_gradient(0, 0, 0);
	EXPECT_EQ(0, f0);

	cmzn_field_id f1 = cmzn_fieldmodule_create_field_gradient(fm, 0, 0);
	EXPECT_EQ(0, f1);

	double values[] = {6.0, 1.0, 2.5};
	cmzn_field_id f2 = cmzn_fieldmodule_create_field_constant(fm, 3, values);

	cmzn_field_id f3 = cmzn_fieldmodule_create_field_gradient(fm, f2, 0);
	EXPECT_EQ(0, f3);

	cmzn_field_id f4 = cmzn_fieldmodule_create_field_gradient(fm, 0, f2);
	EXPECT_EQ(0, f4);

	cmzn_field_destroy(&f0);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&f2);
	cmzn_field_destroy(&f3);
	cmzn_field_destroy(&f4);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

TEST(cmzn_fieldmodule_create_field_gradient, valid_args)
{
	cmzn_context_id context = cmzn_context_create("test");
	cmzn_region_id root_region = cmzn_context_get_default_region(context);
	cmzn_fieldmodule_id fm = cmzn_region_get_fieldmodule(root_region);

	cmzn_region_read_file(root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));

	cmzn_field_id f1 = cmzn_fieldmodule_find_field_by_name(fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), f1);

	cmzn_field_id mag = cmzn_fieldmodule_create_field_magnitude(fm, f1);

	cmzn_field_id f2 = cmzn_fieldmodule_create_field_gradient(fm, mag, f1);
	EXPECT_NE(static_cast<cmzn_field *>(0), f2);

	cmzn_fieldcache_id fc = cmzn_fieldmodule_create_fieldcache(fm);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(fm, 3);
	EXPECT_NE(static_cast<cmzn_mesh *>(0), mesh);

	cmzn_element_id el = cmzn_mesh_find_element_by_identifier(mesh, 1);
	EXPECT_NE(static_cast<cmzn_element *>(0), el);

	double chart_coordinates[] = {0.0, 1.0, 0.0};
	int result = cmzn_fieldcache_set_mesh_location(fc, el, 3, chart_coordinates);
	EXPECT_EQ(CMZN_OK, result);

	double outvalues[3];
	result = cmzn_field_evaluate_real(f2, fc, 3, outvalues);
	EXPECT_EQ(CMZN_OK, result);
	EXPECT_EQ(0.0, outvalues[0]);
	EXPECT_EQ(1.0, outvalues[1]);
	EXPECT_EQ(0.0, outvalues[2]);

	cmzn_element_destroy(&el);
	cmzn_mesh_destroy(&mesh);
	cmzn_field_destroy(&f1);
	cmzn_field_destroy(&mag);
	cmzn_field_destroy(&f2);
	cmzn_fieldcache_destroy(&fc);
	cmzn_fieldmodule_destroy(&fm);
	cmzn_region_destroy(&root_region);
	cmzn_context_destroy(&context);
}

// Issue 3317: Gradient field calculations for grid based scalar fields are not
// being scaled by the number of grid points in each xi direction. The resulting
// gradients are smaller than their correct values.
TEST(cmzn_field, issue_3317_grid_derivatives)
{
	ZincTestSetup zinc;

	int result;
	result = cmzn_region_read_file(zinc.root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_RESOURCE));
	EXPECT_EQ(CMZN_OK, result);
	result = cmzn_region_read_file(zinc.root_region, TestResources::getLocation(TestResources::FIELDMODULE_CUBE_GRID_RESOURCE));
	EXPECT_EQ(CMZN_OK, result);

	cmzn_mesh_id mesh = cmzn_fieldmodule_find_mesh_by_dimension(zinc.fm, 3);
	EXPECT_NE((cmzn_mesh_id)0, mesh);

	cmzn_differentialoperator_id d_dxi1 = cmzn_mesh_get_chart_differentialoperator(mesh, /*order*/1, /*term*/1);
	EXPECT_NE((cmzn_differentialoperator_id)0, d_dxi1);
	cmzn_differentialoperator_id d_dxi2 = cmzn_mesh_get_chart_differentialoperator(mesh, /*order*/1, /*term*/2);
	EXPECT_NE((cmzn_differentialoperator_id)0, d_dxi2);
	cmzn_differentialoperator_id d_dxi3 = cmzn_mesh_get_chart_differentialoperator(mesh, /*order*/1, /*term*/3);
	EXPECT_NE((cmzn_differentialoperator_id)0, d_dxi3);

	cmzn_field_id coordinates = cmzn_fieldmodule_find_field_by_name(zinc.fm, "coordinates");
	EXPECT_NE(static_cast<cmzn_field *>(0), coordinates);
	cmzn_field_id potential = cmzn_fieldmodule_find_field_by_name(zinc.fm, "potential");
	EXPECT_NE(static_cast<cmzn_field *>(0), potential);

	cmzn_fieldcache_id cache = cmzn_fieldmodule_create_fieldcache(zinc.fm);
	cmzn_element_id element = cmzn_mesh_find_element_by_identifier(mesh, 1);
	const double chartLocation[] = { 0.25, 1.0/6.0, 0.125 };
	cmzn_fieldcache_set_mesh_location(cache, element, 3, chartLocation);
	cmzn_element_destroy(&element);

	double outValue;
	EXPECT_EQ(CMZN_OK, result = cmzn_field_evaluate_real(potential, cache, 1, &outValue));
	ASSERT_DOUBLE_EQ(1.75, outValue);
	EXPECT_EQ(CMZN_OK, result = cmzn_field_evaluate_derivative(potential, d_dxi1, cache, 1, &outValue));
	ASSERT_DOUBLE_EQ(2.0, outValue);
	EXPECT_EQ(CMZN_OK, result = cmzn_field_evaluate_derivative(potential, d_dxi2, cache, 1, &outValue));
	ASSERT_DOUBLE_EQ(1.5, outValue);
	EXPECT_EQ(CMZN_OK, result = cmzn_field_evaluate_derivative(potential, d_dxi3, cache, 1, &outValue));
	ASSERT_DOUBLE_EQ(8.0, outValue);

	cmzn_field_id grad_potential = cmzn_fieldmodule_create_field_gradient(zinc.fm, potential, coordinates);
	EXPECT_NE(static_cast<cmzn_field *>(0), grad_potential);

	double outValues[3];
	EXPECT_EQ(CMZN_OK, result = cmzn_field_evaluate_real(grad_potential, cache, 3, outValues));
	ASSERT_DOUBLE_EQ(2.0, outValues[0]);
	ASSERT_DOUBLE_EQ(1.5, outValues[1]);
	ASSERT_DOUBLE_EQ(8.0, outValues[2]);

	cmzn_field_destroy(&grad_potential);
	cmzn_fieldcache_destroy(&cache);
	cmzn_field_destroy(&potential);
	cmzn_field_destroy(&coordinates);
	cmzn_differentialoperator_destroy(&d_dxi1);
	cmzn_differentialoperator_destroy(&d_dxi2);
	cmzn_differentialoperator_destroy(&d_dxi3);
	cmzn_mesh_destroy(&mesh);
}

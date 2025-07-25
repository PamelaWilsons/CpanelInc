//
// Created by cristobal on 29-08-21.
//
/*
MIT License

Copyright (c) 2021 Cristobal Miranda T.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
            */
#include <gtest/gtest.h>

extern "C" {
#include <block.h>
#include <block_frontier.h>
#include <block_topology.h>
#include <k2node.h>
#include <queries_state.h>
}

TEST(k2node_delete_test, simple_test_1) {

  struct k2node *root_node = create_k2node();

  struct k2qstate st;
  TREE_DEPTH_T treedepth = 32;
  TREE_DEPTH_T cutdepth = 10;
  init_k2qstate(&st, treedepth, 256, cutdepth);

  const unsigned long insert_size = 16;

  int already_exists;
  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {
      k2node_insert_point(root_node, i, j, &st, &already_exists);
    }

  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {
      int err = k2node_delete_point(root_node, i, j, &st, &already_exists);
      if (err) {
        std::cout << "error deleting point :  " << err << std::endl;
        exit(1);
      }
    }

  int does_exist;
  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {
      k2node_has_point(root_node, i, j, &st, &does_exist);
      ASSERT_FALSE(does_exist);
    }

  free_rec_k2node(root_node, 0, st.cut_depth);
  clean_k2qstate(&st);
}

TEST(k2node_delete_test, only_delete_existing_points_test) {

  struct k2node *root_node = create_k2node();

  struct k2qstate st;
  TREE_DEPTH_T treedepth = 32;
  TREE_DEPTH_T cutdepth = 10;
  init_k2qstate(&st, treedepth, 256, cutdepth);

  const unsigned long insert_size = 16;

  int already_exists;
  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {
      k2node_insert_point(root_node, i, j, &st, &already_exists);
      ASSERT_FALSE(already_exists);
    }
  int already_not_exists;

  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {

      for (unsigned long k = 0; k < 10; k++) {
        int err =
            k2node_delete_point(root_node, i, j, &st, &already_not_exists);
        if (err) {
          std::cout << "error deleting point :  " << err << std::endl;
          exit(1);
        }
        if (k > 0)
          ASSERT_TRUE(already_not_exists)
              << "failed at i = " << i << ", j = " << j << " k > 0 ";
        else
          ASSERT_FALSE(already_not_exists)
              << "failed at i = " << i << ", j = " << j << " k == 0 ";
      }
    }

  int does_exist;
  for (unsigned long i = 0; i < insert_size; i++)
    for (unsigned long j = 0; j < insert_size; j++) {
      k2node_has_point(root_node, i, j, &st, &does_exist);
      ASSERT_FALSE(does_exist);
    }

  free_rec_k2node(root_node, 0, st.cut_depth);
  clean_k2qstate(&st);
}
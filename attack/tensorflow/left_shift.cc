#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;

REGISTER_OP("LeftShift")
    .Input("to_left_shift: int32")
    .Output("left_shifted: int32")
    .Attr("number_of_bits: int")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    });

#include "tensorflow/core/framework/op_kernel.h"

using namespace tensorflow;

class LeftShiftOp : public OpKernel {
 public:
  explicit LeftShiftOp(OpKernelConstruction* context) : OpKernel(context) {
  // Get the number_of_bits
    OP_REQUIRES_OK(context,
                   context->GetAttr("number_of_bits", &number_of_bits));
  }
  private:
  int number_of_bits;



  void Compute(OpKernelContext* context) override {
    // Grab the input tensor
    const Tensor& input_tensor = context->input(0);
    auto input = input_tensor.flat<int32>();


    // Create an output tensor
    Tensor* output_tensor = NULL;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    auto output_flat = output_tensor->flat<int32>();


    const int N = input.size();
    for (int i = 0; i < N; i++) {
      output_flat(i) = input(i) << number_of_bits;
    }

  }
};

REGISTER_KERNEL_BUILDER(Name("LeftShift").Device(DEVICE_CPU), LeftShiftOp);



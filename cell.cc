// Copyright PinaPL
//
// cell.cc
// PinaPL
//
#include <math.h>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <stdexcept>
#include "weightsLSTM.hh"
#include "cell.hh"
#include "functions.hh"

Cell::Cell(WeightsLSTM* weights) {
    this->weights = weights;
}

std::vector<Eigen::VectorXd> Cell::compute(Eigen::VectorXd previous_output,
                                           Eigen::VectorXd previous_cell_state,
                                           Eigen::VectorXd input) {

    // We keep those values for later
    this->input = input;
    this->previous_output = previous_output;

    // Computes the input gate output
    this->input_gate_out =
        (this->weights->W_in_input_gate * input
        + this->weights->W_prev_input_gate * previous_cell_state)
        .unaryExpr(&sigmoid);

    // Computes the input bloc output
    this->input_block_out =
        (this->weights->W_in_input_block * input
        + this->weights->W_prev_input_block * previous_cell_state)
        .unaryExpr(&tanhyp);

    // Computes the output gate output
    this->output_gate_out =
        (this->weights->W_in_output_gate * input
        + this->weights->W_prev_output_gate * previous_cell_state)
        .unaryExpr(&sigmoid);

    // Computes the new cell state
    this->cell_state =
        (previous_cell_state/*.cwiseProduct(this->forget_gate_out)*/
        + this->input_gate_out.cwiseProduct(this->input_block_out));

    // Computes the cell output
    this->cell_out =
        this->cell_state.unaryExpr(&tanhyp).cwiseProduct(this->output_gate_out);

    // Craft the result to return [cell_out, cell_state]
    std::vector<Eigen::MatrixXd> result;
    result.push_back(cell_out);
    result.push_back(cell_state);
    return result;
}

std::vector<Eigen::VectorXd> Cell::compute_gradient(Eigen::VectorXd* deltas,
    Eigen::VectorXd* previous_delta_cell_in,
    Eigen::VectorXd* previous_delta_cell_state) {
    // TODO : returns [delta_input, delta_cell_state]
}

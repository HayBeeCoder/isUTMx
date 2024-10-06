import React, { forwardRef, useEffect, useState } from 'react';
import {
  LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer
} from 'recharts';

const Graph = ({ data, test_type, id }) => {
  console.log({ data })
  const [x_axis, set_x_axis] = useState("")
  const [y_axis, set_y_axis] = useState("")

  useEffect(() => {
    if (test_type?.toLowerCase() == "torsion") {
      set_x_axis("Angular displacement (°)")

      set_y_axis("Torque (Nm)")
    } else if (test_type?.toLowerCase() == "tension") {
      set_x_axis("Extension (mm)")
      set_y_axis("Force (N)")
    } else if (test_type?.toLowerCase() == "compression") {
      set_x_axis("Compression (mm)")
      set_y_axis("Force (N)")
    }


  }, [test_type])
  const yAxis = data[0]?.force_in_newton
  const xAxis = data[0]?.displacement_in_mm
  // Mock data

  const maxExtension = Math.max(...data.map(d => d?.displacement_in_mm));
  const maxForce = Math.max(...data.map(d => d?.force_in_newton));

  return (
    <ResponsiveContainer width="100%" height={"100%"} id={id}>
      <LineChart
      id="graph"
        data={data}
        margin={{
          top: 10, right: 30, left: 10, bottom: 10,
        }}
      >
        <CartesianGrid strokeDasharray="5 5" />
        <XAxis dataKey="displacement_in_mm" label={{ value: "Extension", position: 'insideBottomRight', offset: -5 }}  // Dynamic domain based on maxExtension
        />
        <YAxis label={{ value: "Force", angle: -90, position: 'insideLeft' }}
          domain={[0, maxForce + 5]} // Dynamic domain based on maxExtension

        />
        <Tooltip />
        {/* <Legend /> */}
        <Line type="monotone" dataKey="force_in_newton" stroke="#8884d8" />
        {/* <Line type="monotone" dataKey="displacement_in_mm" stroke="#8884d8" /> */}
      </LineChart>
    </ResponsiveContainer>
  );
}

export default Graph;

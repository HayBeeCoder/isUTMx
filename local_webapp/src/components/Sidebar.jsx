import React from 'react'
import Status from './Status'
import DataCard from './DataCard'

const Sidebar = ({status, dashboard_info}) => {
    return (
        <>
            <Status status={status} />
            <div className=' md:flex  flex-col gap-4 w-full '>
                <DataCard title="Test type" value={dashboard_info["test_type"] || "--"} />
                <DataCard title="Load Cell's Rating  (N)" value={dashboard_info["sensor_rating_kg"] || "--"} />
                <DataCard title="Target Force (N)" value={dashboard_info["target_force"] || "--"} />
                <DataCard title="Target Extension (N)" value={dashboard_info["target_extension"] || "--"} />
                {/* <DataCard title="Original Length (mm)" value={data.originalLength} /> */}

            </div>
        </>
    )
}

export default Sidebar
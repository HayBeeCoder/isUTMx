import { useState } from 'react'
import Sidebar from './Sidebar'
import Table from "./Table"
import Graph from "./Graph"

function TestPage() {
  const [dashboard_info, set_dashboard_info] = useState({})
  const [test_info, set_test_info] = useState([])
  return (
    <section className='pb-8'>
      <p className='font-faster font-normal text-4xl md:text-4xl text-primary text-center '>
        isutmx
      </p>
      <section className='flex flex-col md:flex-row  gap-7  px-2 '>
        <div className='md:fixed md:top-1/2 md:-translate-y-1/2 justify-self-start md:w-2/12  flex-1 flex flex-col-reverse md:flex-col items-start gap-8'>

          <Sidebar dashboard_info={dashboard_info} />
        </div>
        <div className='w-full md:w-8/12 md:mx-auto   gap-6 md:gap-8  ' id={"content-id"}>
          {/* remove mockData */}

          <div className='h-[50vh] flex-1 gap-6  flex-shrink-0 w-full md:h-[70vh] flex flex-col md:justify-between  overflow-x-scroll md:overflow-x-auto'  >
            <div className=' w-full h-[80%]  md:h-full  ' id='graph' >

              <Graph data={test_info} test_type={dashboard_info["test_type"]} />

            </div>



          </div>
        </div>
        <div className='md:fixed  right-0  w-full md:w-2/12  flex-1 flex flex-col-reverse md:flex-col items-start gap-8'>
          <Table test_info={test_info} dashboard_info={dashboard_info} />

        </div>
      </section>
    </section >
  )
}


export default TestPage
